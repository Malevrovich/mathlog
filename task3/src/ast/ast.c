#include "ast.h"
#include "ast_build.h"

#include <string.h>

void deep_free_ast(struct AST* node) {
    if(!node) return;
    if(node->type == AST_UNARY) free_ast(&(node->as_un.operand));
    if(node->type == AST_BINARY) { free_ast(&(node->as_bin.lhs)); free_ast(&(node->as_bin.rhs)); }
    if(node->type == AST_LITERAL) { free(node->as_lit.value); }
    free(node);
}

void free_ast(struct AST **node) {
    if(!node || !(*node)) return;
    if((*node)->type == AST_UNARY) free_ast(&((*node)->as_un.operand));
    if((*node)->type == AST_BINARY) { free_ast(&((*node)->as_bin.lhs)); free_ast(&((*node)->as_bin.rhs)); }
    if((*node)->type == AST_LITERAL) { free((*node)->as_lit.value); }
    free(*node);
    *node = NULL;
}

bool is_ast_equal(struct AST *lhs, struct AST *rhs) {
    if(lhs->type == AST_NONE) return rhs->type == AST_NONE;
    if(lhs->type == AST_UNARY) 
        return rhs->type == AST_UNARY && 
                (lhs->as_un.type == rhs->as_un.type) &&
                is_ast_equal(lhs->as_un.operand, rhs->as_un.operand);
    if(lhs->type == AST_BINARY) 
        return rhs->type == AST_BINARY &&
                (lhs->as_bin.type == rhs->as_bin.type) && 
                is_ast_equal(lhs->as_bin.lhs, rhs->as_bin.lhs) && 
                is_ast_equal(lhs->as_bin.rhs, rhs->as_bin.rhs);
    if(lhs->type == AST_LITERAL) 
        return rhs->type == AST_LITERAL && 
                (strcmp(lhs->as_lit.value, rhs->as_lit.value) == 0) &&
                (lhs->as_lit.idx == rhs->as_lit.idx);
    if(lhs->type == AST_PATTERN) return rhs->type == AST_PATTERN && (lhs->as_pattern.idx == rhs->as_pattern.idx);
    return false;
}

struct AST *deep_copy_ast(struct AST *node) {
    struct AST *res = create_node();
    *res = *node;
    if(node->type == AST_LITERAL) {
        res->as_lit.value = calloc(strlen(node->as_lit.value), sizeof(char));
        strcat(res->as_lit.value, node->as_lit.value);
    }
    if(node->type == AST_UNARY) {
        res->as_un.operand = deep_copy_ast(node->as_un.operand);
    }
    if(node->type == AST_BINARY) {
        res->as_bin.lhs = deep_copy_ast(node->as_bin.lhs);
        res->as_bin.rhs = deep_copy_ast(node->as_bin.rhs);
    }
    return res;
}
