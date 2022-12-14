#include "tokenize.h"
#include "tokenize_debug.h"

#include <stdio.h>

DEFINE_LIST(token, struct token)
DEFINE_LIST_DEBUG(token, struct token)

const char *TOKEN_STRING[] = {
    [TOKEN_AND] = "AND",
    [TOKEN_IMPLICATION] = "IMPL",
    [TOKEN_L_PARENTHESIS] = "L_PAR",
    [TOKEN_R_PARENTHESIS] = "R_PAR",
    [TOKEN_OR] = "OR",
    [TOKEN_NOT] = "NOT",
    [TOKEN_ERROR] = "ERROR!",
    [TOKEN_END] = "END!",
    [TOKEN_VARIABLE] = "VAR"
};

void token_print(struct token *token) {
    printf("| %s ", TOKEN_STRING[token->type]);
    if(token->type == TOKEN_VARIABLE) printf(" %s ", token->value);
    printf("| --->");
}

void print_token_list(struct list_token **list) {
    list_token_print(list, token_print);
}