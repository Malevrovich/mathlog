#pragma once

#ifndef _AST_H_
#define _AST_H_

#include "tokenize.h"

#include <stdlib.h>

enum AST_type {
    AST_BINARY, AST_UNARY, AST_LITERAL, AST_PATTERN, AST_NONE
};

enum binop_type {
    BIN_AND, BIN_OR, BIN_IMPLICATION
};

enum unop_type { 
    UN_NOT
};

enum lit_type { 
    LIT_VAR 
};

struct AST {
    enum AST_type type;
    union {
        struct binary {
            enum binop_type type;
            struct AST *lhs;
            struct AST *rhs;
        } as_bin;
        struct unary{
            enum unop_type type;
            struct AST *operand;
        } as_un;
        struct literal{
            enum lit_type type;
            size_t idx;
            char *value;
        } as_lit;
        struct pattern{
            size_t idx;
        } as_pattern;
    };
};

void deep_free_ast(struct AST* node);
void free_ast(struct AST **node);
struct AST *deep_copy_ast(struct AST *node);

bool is_ast_equal(const struct AST *lhs, const struct AST *rhs);

#endif /* _AST_H_ */
