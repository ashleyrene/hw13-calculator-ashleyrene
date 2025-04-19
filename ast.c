#include "ast.h"
#include <assert.h>
#include <stdlib.h>

struct AST_node *to_ast(struct CST_node *cst) { return NULL; }
void free_ast(struct AST_node *ast) {}
int interpret(struct AST_node *ast) { return -1; }
