#include "ast_index.h"
#include "ast.h"
#include "list.h"

#include <string.h>

DECLARE_LIST(string, char *)
DEFINE_LIST(string, char *)

bool eq(char *lhs, char *rhs) {
    return strcmp(lhs, rhs) == 0;
}

static size_t linear_dfs_index(struct AST *root, struct list_string **table, size_t size) {
    if(!root) return size;
    if(root->type == AST_UNARY) return linear_dfs_index(root->as_un.operand, table, size);
    if(root->type == AST_BINARY) {
        size_t lhs_size = linear_dfs_index(root->as_bin.lhs, table, size);
        size_t res_size = linear_dfs_index(root->as_bin.rhs, table, lhs_size);
        return res_size;
    }
    if(root->type == AST_LITERAL) {
        size_t idx = list_string_indexof(table, root->as_lit.value, eq);
        if(idx == SIZE_MAX) {
            list_string_push_back(table, root->as_lit.value);
            root->as_lit.idx = size;
            return size + 1;
        }
        root->as_lit.idx = idx;
        return size;
    }
    return size;
}

size_t index(struct AST *root) {
    struct list_string **table = malloc(sizeof(struct list_string *));
    *table = NULL;
    
    size_t res = linear_dfs_index(root, table, 0);

    list_string_free(table);
    free(table);

    return res;
}