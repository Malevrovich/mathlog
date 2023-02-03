/**
 * @file ast.h
 * @brief Internal representation of logical formulas in form of Abstract Syntax Tree
 */
#pragma once

#ifndef _AST_H_
#define _AST_H_

#include "tokenize.h"

#include <stdlib.h>

/**
 * @brief Type of AST node
 */
enum AST_type {
    AST_BINARY, AST_UNARY, AST_LITERAL, AST_PATTERN, AST_NONE
};

/**
 * @brief Type of AST_BINARY node
 */
enum binop_type {
    BIN_AND, BIN_OR, BIN_IMPLICATION
};

/**
 * @brief Type of AST_UNARY node
 */
enum unop_type { 
    UN_NOT
};

/**
 * @brief Type of AST_LITERAL node
 */
enum lit_type { 
    LIT_VAR 
};

/**
 * @brief Abstract Syntax Tree node
 */
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

/**
 * @brief Free children and then node
 * @param [in] node Node to free
 */
void deep_free_ast(struct AST* node);
/**
 * @brief Free children and then node, put NULL by the node's pointer
 * @param [in] node Pointer to node to free
 */
void free_ast(struct AST **node);
/**
 * @brief Creates copy of node and it's children
 * @param [in] node Node to copy 
 * @return struct AST* copy
 */
struct AST *deep_copy_ast(struct AST *node);

/**
 * @brief Check if ASTs are equal by value
 * @param [in] lhs 
 * @param [in] rhs 
 * @return true Trees are equal 
 * @return false Trees are not equal
 */
bool is_ast_equal(struct AST *lhs, struct AST *rhs);

#endif /* _AST_H_ */
