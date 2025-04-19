#include "ast.h"
#include "cst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_CST(struct CST_node *cst) {
  switch (cst->type) {
  case NODE_EXPR:
    printf("expr(");
    print_CST(cst->expr.term);
    printf(", ");
    print_CST(cst->expr.expr_rest);
    printf(")");
    break;
  case NODE_EXPR_REST:
    printf("expr_rest(");
    if (cst->expr_rest.expr != NULL) {
      printf("%c, ", cst->expr_rest.op);
      print_CST(cst->expr_rest.expr);
    }
    printf(")");
    break;
  case NODE_TERM:
    printf("term(");
    print_CST(cst->term.factor);
    printf(", ");
    print_CST(cst->term.term_rest);
    printf(")");
    break;
  case NODE_TERM_REST:
    printf("term_rest(");
    if (cst->term_rest.term != NULL) {
      printf("%c, ", cst->term_rest.op);
      print_CST(cst->term_rest.term);
    }
    printf(")");
    break;
  case NODE_FACTOR:
    if (cst->factor.negated)
      printf("-");
    printf("factor(");
    if (cst->factor.expr != NULL) {
      print_CST(cst->factor.expr);
    } else {
      printf("lit(%d)", cst->factor.literal);
    }
    printf(")");
  }
}

static void print_AST(struct AST_node *ast) {
  switch (ast->type) {
  case NODE_ADD:
    printf("(");
    print_AST(ast->left);
    printf(" + ");
    print_AST(ast->right);
    printf(")");
    break;
  case NODE_MUL:
    printf("(");
    print_AST(ast->left);
    printf(" * ");
    print_AST(ast->right);
    printf(")");
    break;
  case NODE_NEG:
    printf("-");
    print_AST(ast->left);
    break;
  case NODE_LITERAL:
    printf("%d", ast->literal);
    break;
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  bool debug = (argc >= 3 && !strcasecmp(argv[2], "debug"));

  char buf[1024];
  fgets(buf, 1024, fp);
  fclose(fp);

  struct stream s = {.text = buf, .pos = 0, .length = strlen(buf)};
  struct CST_node *parse_tree = parse_expr(&s);
  if (parse_tree == NULL) {
    printf("Invalid\n");
  } else {
    struct AST_node *ast = to_ast(parse_tree);

    if (debug) {
      print_CST(parse_tree);
      print_AST(ast);
    }

    printf("Result: %d\n", interpret(ast));
    free_cst(parse_tree);
    free_ast(ast);
  }

  return EXIT_SUCCESS;
}
