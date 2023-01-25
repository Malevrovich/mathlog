#include "compute.h"

#include "ast.h"
#include "ast_index.h"

#include <stdio.h>

static bool compute_vals(struct AST * restrict root, bool *vals) {
    if(!root) return false;
    if(root->type == AST_NONE) return false;
    if(root->type == AST_LITERAL) return vals[root->as_lit.idx];
    if(root->type == AST_UNARY) {
        if(root->as_un.type == UN_NOT) return !compute_vals(root->as_un.operand, vals);
    }
    if(root->type == AST_BINARY) {
        if(root->as_bin.type == BIN_AND) 
            return compute_vals(root->as_bin.lhs, vals) && compute_vals(root->as_bin.rhs, vals);
        if(root->as_bin.type == BIN_OR)
            return compute_vals(root->as_bin.lhs, vals) || compute_vals(root->as_bin.rhs, vals);
        if(root->as_bin.type == BIN_IMPLICATION) 
            return !compute_vals(root->as_bin.lhs, vals) || compute_vals(root->as_bin.rhs, vals);
    }
    return false;
}

static struct compute_res compute_dfs(struct AST * restrict root, bool *vals, size_t idx) {
    if(idx == 0) {
        struct compute_res res = (struct compute_res) {0,0};
        vals[idx] = false;
        if(compute_vals(root, vals)) res.true_cnt++;
        else res.false_cnt++;

        vals[idx] = true;
        if(compute_vals(root, vals)) res.true_cnt++;
        else res.false_cnt++;

        return res;
    }

    vals[idx] = false;
    struct compute_res false_res = compute_dfs(root, vals, idx - 1);
    
    vals[idx] = true;
    struct compute_res true_res = compute_dfs(root, vals, idx - 1);
    return (struct compute_res) {.true_cnt = false_res.true_cnt + true_res.true_cnt,
                                 .false_cnt = false_res.false_cnt + true_res.false_cnt};
}

struct compute_res compute(struct AST * restrict root) {
    size_t table_size = index(root);
    
    bool *vals = malloc(table_size * sizeof(bool));

    struct compute_res res = compute_dfs(root, vals, table_size - 1);

    free(vals);
    return res;
}