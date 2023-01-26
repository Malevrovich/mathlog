#pragma once

#ifndef _AST_PARSE_H_
#define _AST_PARSE_H_

#include "list.h"
#include "tokenize.h"

struct parse_ast_res {
    struct AST *val;
    enum {
        PARSE_AST_SUCCESS, PARSE_AST_TOKEN_ERR, PARSE_AST_INVALID_SEQUENCE,
        PARSE_AST_NULL_POINTER, PARSE_AST_UNKNOWN_TOKEN
    } status;
};

extern const char *PARSE_AST_RES_STRING[];

static const struct parse_ast_res PARSE_INVALID_SEQUENCE = (struct parse_ast_res) { NULL, PARSE_AST_INVALID_SEQUENCE };
static const struct parse_ast_res PARSE_NULL_POINTER = (struct parse_ast_res) { NULL, PARSE_AST_NULL_POINTER };
static const struct parse_ast_res PARSE_UNKNOWN_TOKEN = (struct parse_ast_res ) { NULL, PARSE_AST_UNKNOWN_TOKEN };

struct parse_ast_res parse_ast(struct list_token **tokens);
struct parse_ast_res parse_ast_from(const char * restrict str);

#endif