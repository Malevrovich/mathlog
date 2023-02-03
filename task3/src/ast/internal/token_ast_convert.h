/**
 * @file token_ast_convert.h
 * @author your name (you@domain.com)
 * @brief Convert token to corresponding AST node
 */
#pragma once

#ifndef _TOKEN_AST_CONVERT_H_
#define _TOKEN_AST_CONVERT_H_

/**
 * @brief Array to convert enum token_type to enum AST_type
 */
static const int TOKENS_AST_TYPE[] = {
    [TOKEN_AND] = AST_BINARY,
    [TOKEN_OR] = AST_BINARY,
    [TOKEN_IMPLICATION] = AST_BINARY,
    [TOKEN_NOT] = AST_UNARY,
    [TOKEN_L_PARENTHESIS] = AST_NONE,
    [TOKEN_VARIABLE] = AST_LITERAL,
    [TOKEN_R_PARENTHESIS] = AST_NONE,
    [TOKEN_END] = AST_NONE,
    [TOKEN_ERROR] = AST_NONE
};

static const int TOKENS_OP_TYPE[] = {
    [TOKEN_AND] = BIN_AND,
    [TOKEN_OR] = BIN_OR,
    [TOKEN_IMPLICATION] = BIN_IMPLICATION,
    [TOKEN_NOT] = UN_NOT,
    [TOKEN_VARIABLE] = LIT_VAR
};

/**
 * @brief Create AST node from token
 * 
 * @param [in] token Token to generate from
 * @return struct AST* node - generated node
 */
struct AST *node_from_token(struct token token);

#endif