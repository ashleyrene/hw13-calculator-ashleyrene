#include "ast.h"
#include <stdlib.h>

static struct AST_node *to_ast_expr(struct CST_node *cst);
static struct AST_node *to_ast_term(struct CST_node *cst);
static struct AST_node *to_ast_factor(struct CST_node *cst);

struct AST_node *to_ast(struct CST_node *cst) {
    if (!cst) return NULL;
    return to_ast_expr(cst);
}

static struct AST_node *to_ast_expr(struct CST_node *cst) {
    if (!cst || cst->type != NODE_EXPR) return NULL;

    struct AST_node *left = to_ast_term(cst->expr.term);
    if (!left) return NULL;

    struct CST_node *rest = cst->expr.expr_rest;
    if (rest && rest->expr_rest.expr) {
        struct AST_node *right = to_ast_expr(rest->expr_rest.expr);
        if (!right) {
            free_ast(left);
            return NULL;
        }

        struct AST_node *node = malloc(sizeof(struct AST_node));
        if (!node) return NULL;
        node->type = AST_ADD;
        node->binary.left = left;
        node->binary.right = right;
        return node;
    }

    return left;
}

static struct AST_node *to_ast_term(struct CST_node *cst) {
    if (!cst || cst->type != NODE_TERM) return NULL;

    struct AST_node *left = to_ast_factor(cst->term.factor);
    if (!left) return NULL;

    struct CST_node *rest = cst->term.term_rest;
    if (rest && rest->term_rest.term) {
        struct AST_node *right = to_ast_term(rest->term_rest.term);
        if (!right) {
            free_ast(left);
            return NULL;
        }

        struct AST_node *node = malloc(sizeof(struct AST_node));
        if (!node) return NULL;
        node->type = AST_MUL;
        node->binary.left = left;
        node->binary.right = right;
        return node;
    }

    return left;
}

static struct AST_node *to_ast_factor(struct CST_node *cst) {
    if (!cst || cst->type != NODE_FACTOR) return NULL;

    if (cst->factor.negated) {
        struct AST_node *child = to_ast_factor(cst->factor.negated);
        if (!child) return NULL;

        struct AST_node *node = malloc(sizeof(struct AST_node));
        if (!node) return NULL;
        node->type = AST_NEG;
        node->unary.child = child;
        return node;
    } else if (cst->factor.expr) {
        return to_ast_expr(cst->factor.expr);
    } else {
        struct AST_node *node = malloc(sizeof(struct AST_node));
        if (!node) return NULL;
        node->type = AST_INT;
        node->value = cst->factor.num;
        return node;
    }
}

void free_ast(struct AST_node *ast) {
    if (!ast) return;

    switch (ast->type) {
        case AST_ADD:
        case AST_MUL:
            free_ast(ast->binary.left);
            free_ast(ast->binary.right);
            break;
        case AST_NEG:
            free_ast(ast->unary.child);
            break;
        case AST_INT:
            break;
    }

    free(ast);
}

long interpret(struct AST_node *ast) {
    if (!ast) return 0;

    switch (ast->type) {
        case AST_INT:
            return ast->value;
        case AST_NEG:
            return -interpret(ast->unary.child);
        case AST_ADD:
            return interpret(ast->binary.left) + interpret(ast->binary.right);
        case AST_MUL:
            return interpret(ast->binary.left) * interpret(ast->binary.right);
    }

    return 0; 
}
