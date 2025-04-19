#ifndef AST_H_
#define AST_H_

#include "cst.h"

enum ast_node_type { NODE_ADD, NODE_MUL, NODE_NEG, NODE_LITERAL };
struct AST_node {
  enum ast_node_type type;
  union {
    struct {
      struct AST_node *left;
      struct AST_node *right;
    };
    int literal;
  };
};

struct AST_node *to_ast(struct CST_node *cst);
int interpret(struct AST_node *ast);
void free_ast(struct AST_node *ast);

#endif // AST_H_
