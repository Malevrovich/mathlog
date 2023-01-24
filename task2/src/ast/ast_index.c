#include "ast_index.h"
#include "ast.h"
#include "list.h"

#include <string.h>

DECLARE_LIST(string, char *)
DEFINE_LIST(string, char *)

bool eq(char *lhs, char *rhs) {
    return strcmp(lhs, rhs) == 0;
}

static size_t linear_dfs_index(struct AST *root, struct list_string **table) {
    if(!root) return 0;
    if(root->type == AST_UNARY) return linear_dfs_index(root->as_un.operand, table);
    if(root->type == AST_BINARY) return linear_dfs_index(root->as_bin.lhs, table) + linear_dfs_index(root->as_bin.rhs, table);
    if(root->type == AST_LITERAL) {
        
        return 1;
    }
    return 0;
}

size_t index(struct AST *root) {
    struct list_string **table = malloc(sizeof(struct list_string *));
    *table = NULL;
    
    size_t res = linear_dfs_index(root, table);

    list_string_free(table);
    free(table);

    return res;
}