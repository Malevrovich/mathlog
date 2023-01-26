#include "match_schemas.h"

#include "ast.h"

#include <string.h>

#define MAX_PATTERN_COUNT_IN_SCHEMA 3

static bool is_match_dfs(struct AST *schema, struct AST *root, struct AST *samples[MAX_PATTERN_COUNT_IN_SCHEMA]) {
    if(!schema) return (!root);
    if(schema->type == AST_PATTERN) {
        if(!samples[schema->as_pattern.idx]) {
            samples[schema->as_pattern.idx] = root;
            return true;
        } else {
            return is_ast_equal(samples[schema->as_pattern.idx], root);
        }
    }
    if(schema->type != root->type) return false;
    if(schema->type == AST_NONE) return true;
    if(schema->type == AST_LITERAL) return (strcmp(schema->as_lit.value, root->as_lit.value) == 0) &&
                                                                    (schema->as_lit.idx == root->as_lit.idx);
    if(schema->type == AST_UNARY) return (schema->as_un.type == root->as_un.type) &&
                                            is_match_dfs(schema->as_un.operand, root->as_un.operand, samples);
    if(schema->type == AST_BINARY) return (schema->as_bin.type == root->as_bin.type) &&
                                            is_match_dfs(schema->as_bin.lhs, root->as_bin.lhs, samples) &&
                                            is_match_dfs(schema->as_bin.rhs, root->as_bin.rhs, samples);
    return false;
}

bool is_match(struct AST *schema, struct AST *root) {
    struct AST *samples[MAX_PATTERN_COUNT_IN_SCHEMA];
    for(size_t i = 0; i < MAX_PATTERN_COUNT_IN_SCHEMA; i++) 
        samples[i] = NULL;
    return is_match_dfs(schema, root, samples);
}