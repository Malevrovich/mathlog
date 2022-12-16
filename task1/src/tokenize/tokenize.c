#include "tokenize.h"

#include <stdio.h>
#include <string.h>

static const char *TOKENS_LITERALS[] = {
    [TOKEN_AND] = "&",
    [TOKEN_IMPLICATION] = "->",
    [TOKEN_L_PARENTHESIS] = "(",
    [TOKEN_R_PARENTHESIS] = ")",
    [TOKEN_OR] = "|",
    [TOKEN_NOT] = "!"
};

DEFINE_LIST(token, struct token)

static inline struct token try_parse_literal(const char ** restrict const str) {
    if(!str || !(*str)) return (struct token) { TOKEN_ERROR, "NULL pointer" };

    for(size_t i = 0; i <= LAST_LITERAL; i++) {
        if(strncmp(*str, TOKENS_LITERALS[i], strlen(TOKENS_LITERALS[i])) == 0) {
            *str += strlen(TOKENS_LITERALS[i]);
            return (struct token) { i, NULL };
        }
    }

    return (struct token) { TOKEN_ERROR, "Expected literal" };
}

static inline bool is_varname_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == 39);
}

static inline bool is_whitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

static inline struct token try_parse_variable(const char ** restrict const str) {
    if(!str || !(*str)) return (struct token) { TOKEN_ERROR, "NULL pointer!" };

    if(**str < 'A' && **str > 'Z') {
        return (struct token) { TOKEN_ERROR, "Expected variable" };
    }

    const char *pos = (*str) + 1;
    while(is_varname_char(*pos)) {
        pos++;
    }

    struct token res;
    
    res.type = TOKEN_VARIABLE;
    res.value = (char *) malloc (sizeof(char) * (pos - *str));
    
    strncpy(res.value, *str, pos - *str);
    *str = pos;

    return res;
}

static inline void skip_whitespaces(const char ** restrict const str) {
    while(is_whitespace(**str)) (*str)++;
}

static struct token parse_token(const char ** restrict const str) {
    skip_whitespaces(str);
    if(!str || !(*str)) return (struct token) {TOKEN_ERROR, "NULL pointer"};

    if(**str == '\0') return (struct token) {TOKEN_END, NULL};

    struct token possible_literal = try_parse_literal(str);
    if(possible_literal.type != TOKEN_ERROR) return possible_literal;

    struct token possible_var = try_parse_variable(str);
    
    if(possible_literal.type == TOKEN_ERROR) possible_literal.value = "Couldn't recognize token";

    return possible_var;
}

struct list_token **tokenize(const char * const str) {
    if(!str) return NULL;

    struct list_token **res = (struct list_token **) malloc(sizeof(struct list_token *));

    const char *pos = str;
    struct token cur = parse_token(&pos);
    while(cur.type != TOKEN_END && cur.type != TOKEN_ERROR) {
        list_token_push_back(res, cur);
        cur = parse_token(&pos);
    }

    if(cur.type == TOKEN_ERROR) {
        list_token_free(res);
        free(res);
        fprintf(stderr, "Tokenization error at position %ld\n", pos - str);
        fprintf(stderr, "%s\n", cur.value);
        return NULL;
    }

    return res;
}

