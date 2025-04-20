#include "cst.h"
#include <stdlib.h>
#include <string.h>

struct CST_node *parse_expr(struct stream *s);
struct CST_node *parse_expr_rest(struct stream *s);
struct CST_node *parse_term(struct stream *s);
struct CST_node *parse_term_rest(struct stream *s);
struct CST_node *parse_factor(struct stream *s);

struct CST_node *parse_expr(struct stream *s) {
    struct CST_node *term = parse_term(s);
    if (!term) return NULL;

    struct CST_node *rest = parse_expr_rest(s);
    if (!rest) {
        free_cst(term);
        return NULL;
    }

    struct CST_node *node = malloc(sizeof(struct CST_node));
    if (!node) return NULL;
    node->type = NODE_EXPR;
    node->expr.term = term;
    node->expr.expr_rest = rest;
    return node;
}

struct CST_node *parse_expr_rest(struct stream *s) {
    if (peek(s) == TOK_ADD) {
        consume(s);
        struct CST_node *expr = parse_expr(s);
        if (!expr) return NULL;

        struct CST_node *node = malloc(sizeof(struct CST_node));
        if (!node) return NULL;
        node->type = NODE_EXPR_REST;
        node->expr_rest.expr = expr;
        return node;
    } else {
        struct CST_node *node = malloc(sizeof(struct CST_node));
        if (!node) return NULL;
        node->type = NODE_EXPR_REST;
        node->expr_rest.expr = NULL;
        return node;
    }
}

struct CST_node *parse_term(struct stream *s) {
    struct CST_node *factor = parse_factor(s);
    if (!factor) return NULL;

    struct CST_node *rest = parse_term_rest(s);
    if (!rest) {
        free_cst(factor);
        return NULL;
    }

    struct CST_node *node = malloc(sizeof(struct CST_node));
    if (!node) return NULL;
    node->type = NODE_TERM;
    node->term.factor = factor;
    node->term.term_rest = rest;
    return node;
}

struct CST_node *parse_term_rest(struct stream *s) {
    if (peek(s) == TOK_MUL) {
        consume(s);
        struct CST_node *term = parse_term(s);
        if (!term) return NULL;

        struct CST_node *node = malloc(sizeof(struct CST_node));
        if (!node) return NULL;
        node->type = NODE_TERM_REST;
        node->term_rest.term = term;
        return node;
    } else {
        struct CST_node *node = malloc(sizeof(struct CST_node));
        if (!node) return NULL;
        node->type = NODE_TERM_REST;
        node->term_rest.term = NULL;
        return node;
    }
}

struct CST_node *parse_factor(struct stream *s) {
    if (peek(s) == TOK_INT) {
        struct token *tok = consume(s);
        if (!tok) return NULL;

        struct CST_node *node = malloc(sizeof(struct CST_node));
        if (!node) return NULL;
        node->type = NODE_FACTOR;
        node->factor.num = atoi(tok->start);
        node->factor.negated = NULL;
        node->factor.expr = NULL;
        return node;
    } else if (peek(s) == TOK_LPAREN) {
        consume(s);
        struct CST_node *expr = parse_expr(s);
        if (!expr) return NULL;

        if (peek(s) != TOK_RPAREN) {
            free_cst(expr);
            return NULL;
        }
        consume(s);

        struct CST_node *node = malloc(sizeof(struct CST_node));
        if (!node) return NULL;
        node->type = NODE_FACTOR;
        node->factor.expr = expr;
        node->factor.negated = NULL;
        return node;
    } else if (peek(s) == TOK_NEG) {
        consume(s);
        struct CST_node *neg = parse_factor(s);
        if (!neg) return NULL;

        struct CST_node *node = malloc(sizeof(struct CST_node));
        if (!node) return NULL;
        node->type = NODE_FACTOR;
        node->factor.negated = neg;
        node->factor.expr = NULL;
        return node;
    }

    return NULL;
}

void free_cst(struct CST_node *cst) {
    if (!cst) return;

    switch (cst->type) {
        case NODE_EXPR:
            free_cst(cst->expr.term);
            free_cst(cst->expr.expr_rest);
            break;
        case NODE_EXPR_REST:
            free_cst(cst->expr_rest.expr);
            break;
        case NODE_TERM:
            free_cst(cst->term.factor);
            free_cst(cst->term.term_rest);
            break;
        case NODE_TERM_REST:
            free_cst(cst->term_rest.term);
            break;
        case NODE_FACTOR:
            free_cst(cst->factor.negated);
            free_cst(cst->factor.expr);
            break;
    }

    free(cst);
}

