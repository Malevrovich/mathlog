#pragma once

#ifndef _TOKENIZE_H_
#define _TOKENIZE_H_

#include "list.h"

struct token {
    enum token_type{
        TOKEN_L_PARENTHESIS, TOKEN_R_PARENTHESIS,
        TOKEN_AND, TOKEN_OR, TOKEN_IMPLICATION, TOKEN_NOT,
        TOKEN_VARIABLE,
        TOKEN_END, TOKEN_ERROR
    } type;
    char *value;
};
static const size_t LAST_LITERAL = 5;

DECLARE_LIST(token, struct token)

struct list_token **tokenize(const char * const str);

#endif
