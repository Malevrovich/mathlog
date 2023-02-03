/**
 * @file ast_build.h
 * @brief Functions to create AST nodes
 */
#pragma once

#ifndef _AST_BUILD_H_
#define _AST_BUILD_H_

#include "ast.h"

/**
 * @brief Allocate memory for node
 * @return struct AST* genarated node
 */
inline struct AST *create_node() { 
    return (struct AST *) malloc(sizeof( struct AST )); 
}

/**
 * @brief Creates node with given body
 * @param [in] body Body to place in new node 
 * @return struct AST* node - generated node
 */
inline struct AST *build_node(struct AST body) {
    struct AST *res = create_node();
    *res = body;
    return res;
}

/**
 * @brief Generates AST body with AST_BINOP type
 * @param [in] type Type of binary operation 
 * @return struct AST node - generated body
 */
inline struct AST make_binop(enum binop_type type) {
    return (struct AST) { .type=AST_BINARY, .as_bin = { type, NULL, NULL }};
}

/**
 * @brief Generates AST body with AST_UNOP type
 * @param [in] type Type of unary operation 
 * @return struct AST node - generated body
 */
inline struct AST make_unop(enum unop_type type) {
    return (struct AST) { .type=AST_UNARY, .as_un = { type, NULL } };
}

/**
 * @brief Generates AST body with AST_LIT type
 * @param [in] value Value of literal 
 * @return struct AST node - generated body
 */
inline struct AST make_var(char *value) {
    return (struct AST) { .type=AST_LITERAL, .as_lit = {.type = LIT_VAR, .value = value } };
}

/**
 * @brief Generates AST body with AST_NONE type
 * @return struct AST node - generated body
 */
inline struct AST make_none() {
    return (struct AST) { .type=AST_NONE };
}

/**
 * @brief Generates AST body with AST_PATTERN type
 * @param [in] idx Index of pattern 
 * @return struct AST node - generated body
 */
inline struct AST make_pattern(size_t idx) {
    return (struct AST) { .type=AST_PATTERN, .as_pattern = { .idx=idx } };
}

#endif /* _AST_BUILD_H_ */
