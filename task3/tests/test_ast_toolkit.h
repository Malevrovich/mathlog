#pragma once

#ifndef _TEST_AST_TOOLKIT_H_
#define _TEST_AST_TOOLKIT_H_

#include "ast.h"

#include <assert.h>
#include <string.h>

#define CHECK_BIN(node, expected_type)              \
do{                                                 \
    assert(node->type == AST_BINARY);               \
    assert(node->as_bin.type == expected_type);     \
} while(0)               

#define CHECK_UN(node, expected_type)               \
do{                                                 \
    assert(node->type == AST_UNARY);                \
    assert(node->as_un.type == expected_type);      \
} while(0)

#define CHECK_LITERAL(node, expected_val)                   \
do {                                                        \
    assert(node->type == AST_LITERAL);                      \
    assert(strcmp(node->as_lit.value, expected_val) == 0);  \
} while(0)

#define CHECK_LITERAL_IDX(node, expected_val, expected_idx) \
do {                                                        \
    assert(node->type == AST_LITERAL);                      \
    assert(strcmp(node->as_lit.value, expected_val) == 0);  \
    assert(node->as_lit.idx == expected_idx);               \
} while(0)


#endif /* _TEST_AST_TOOLKIT_H_ */