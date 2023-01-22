#pragma once

#ifndef _AST_BUILD_H_
#define _AST_BUILD_H_

#include "ast.h"

inline struct AST *create_node() { 
    return (struct AST *) malloc(sizeof( struct AST )); 
}

inline struct AST make_binop(enum binop_type type) {
    return (struct AST) { .type=AST_BINARY, .as_bin = { type, NULL, NULL }};
}

inline struct AST make_unop(enum unop_type type) {
    return (struct AST) { .type=AST_UNARY, .as_un = { type, NULL } };
}

inline struct AST make_var(char *value) {
    return (struct AST) { .type=AST_LITERAL, .as_lit = {.type = LIT_VAR, .value = value } };
}

inline struct AST make_none() {
    return (struct AST) { .type=AST_NONE };
}

#endif /* _AST_BUILD_H_ */
