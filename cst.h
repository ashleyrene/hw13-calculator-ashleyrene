#ifndef CST_H_
#define CST_H_

#include "tokenizer.h"

#include <stdbool.h>

enum cst_node_type { NODE_EXPR, NODE_EXPR_REST, NODE_TERM, NODE_TERM_REST, NODE_FACTOR };
struct CST_node {
  enum cst_node_type type;
  union {
    struct {
      struct CST_node *term;
      struct CST_node *expr_rest;
    } expr;

    struct {
      char op;
      struct CST_node *expr; // suggestion: set this to NULL if its empty
    } expr_rest;

    struct {
      struct CST_node *factor;
      struct CST_node *term_rest; // suggestion: set this to NULL if its empty
    } term;

    struct {
      char op;
      struct CST_node *term;
    } term_rest;

    struct {
      int literal;
      struct CST_node *expr; // suggestion: set this to NULL if its a literal
      bool negated;
    } factor;
  };
};

struct CST_node *parse_expr(struct stream *s);
struct CST_node *parse_expr_rest(struct stream *s);

struct CST_node *parse_term(struct stream *s);
struct CST_node *parse_term_rest(struct stream *s);

struct CST_node *parse_factor(struct stream *s);

void free_cst(struct CST_node *cst);

#endif // CST_H_
