#include "tokenize.h"
#include "ast.h"
#include "ast_parse.h"
#include "ast_debug.h"
#include "tokenize_debug.h"

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE (1024 * 100 + 1)

DEFINE_LIST(token, struct token);

int main() {
    char *buf = calloc(BUFFER_SIZE, sizeof(char));
    
    if(!fgets(buf, BUFFER_SIZE, stdin)) {
        fprintf(stderr, "Unable to read input line\n");
        free(buf);
        return 1;
    }

    struct list_token **tokens = tokenize(buf);

    if(!tokens) {
        fprintf(stderr, "Tokenization error\n");
        free(buf);
        return 1;
    }



    struct parse_ast_res parse_res = parse_ast(tokens);

    if(parse_res.status != PARSE_AST_SUCCESS) {
        fprintf(stderr, "Parsing failed\n");
        fprintf(stderr, "%s\n", PARSE_AST_RES_STRING[parse_res.status]);

        free(buf);
        list_token_free(tokens);
        free(tokens);

        return 1;
    }

    print_ast(stdout, parse_res.val);
    printf("\n");

    return 0;
}