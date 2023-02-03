/**
 * @file tokenize.h
 * @brief Provides tokenization
 */
#pragma once

#ifndef _TOKENIZE_H_
#define _TOKENIZE_H_

#include "list.h"

/** 
 * @brief Atomic element of text's internal representation
*/
struct token {
    /**
     * @brief Type of token
     */
    enum token_type{
        TOKEN_L_PARENTHESIS, TOKEN_R_PARENTHESIS,
        TOKEN_AND, TOKEN_OR, TOKEN_IMPLICATION, TOKEN_NOT,
        TOKEN_VARIABLE,
        TOKEN_END, TOKEN_ERROR
    } type;
    char *value;
};
/** 
 * Number of enum token_type element that represents literal
*/
static const size_t LAST_LITERAL = 5;

DECLARE_LIST(token, struct token)

/**
 * @brief Tokenize the input string
 * 
 * Convert the string to tokens
 * 
 * @param [in] str String that will be tokenized
 * @return struct list_token** - list of tokens
 */
struct list_token **tokenize(const char * const str);

#endif
