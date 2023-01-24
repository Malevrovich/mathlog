#include "ast.h"

void deep_free_ast(struct AST* node) {
    if(node->type == AST_UNARY) deep_free_ast(node->as_un.operand);
    if(node->type == AST_BINARY) { deep_free_ast(node->as_bin.lhs); deep_free_ast(node->as_bin.rhs); }
    if(node->type == AST_LITERAL) { free(node->as_lit.value); }
    free(node);
}