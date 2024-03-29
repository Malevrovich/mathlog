#include "test_utils.h"
#include "list.h"
#include "tokenize.h"
#include "tokenize_debug.h"

#include <assert.h>
#include <string.h>

// #define DISABLED

DEFINE_LIST(token, struct token)

static inline void check_token(struct token expected, struct token real) {
    assert(expected.type == real.type);
    if(expected.value == NULL) assert(real.value == NULL);
    else assert(strcmp(expected.value, real.value) == 0);
}

#define CHECK_NEXT_TOKEN(list, expected_type, expected_value)   \
do {                                                            \
    struct token real = list_token_pop_front(list);             \
    struct token expected = { expected_type, expected_value};   \
    assert(real.type == expected.type);                         \
    if(expected.value == NULL) assert(real.value == NULL);      \
    else assert(strcmp(expected.value, real.value) == 0);       \
} while(0)

TEST(simple) {
    char *str = "A | B & C -> !D & (A10A) -> !A12";
    struct list_token **res = tokenize(str);

    print_token_list(res);
    printf("\n");

    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "A");
    CHECK_NEXT_TOKEN(res, TOKEN_OR, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "B");
    CHECK_NEXT_TOKEN(res, TOKEN_AND, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "C");
    CHECK_NEXT_TOKEN(res, TOKEN_IMPLICATION, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_NOT, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "D");
    CHECK_NEXT_TOKEN(res, TOKEN_AND, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_L_PARENTHESIS, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "A10A");
    CHECK_NEXT_TOKEN(res, TOKEN_R_PARENTHESIS, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_IMPLICATION, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_NOT, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "A12");

    assert(list_token_empty(res));
    free(res);
}

TEST(from_ast) {
    char *str = "D -> C | !B & A";
    struct list_token **res = tokenize(str);

    print_token_list(res);
    printf("\n");

    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "D");
    CHECK_NEXT_TOKEN(res, TOKEN_IMPLICATION, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "C");
    CHECK_NEXT_TOKEN(res, TOKEN_OR, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_NOT, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "B");
    CHECK_NEXT_TOKEN(res, TOKEN_AND, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "A");

    assert(list_token_empty(res));
    free(res);
}

TEST(from_ast_priority) {
    char *str = "A & B | C -> D";
    struct list_token **res = tokenize(str);

    print_token_list(res);
    printf("\n");

    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "A");
    CHECK_NEXT_TOKEN(res, TOKEN_AND, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "B");
    CHECK_NEXT_TOKEN(res, TOKEN_OR, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "C");
    CHECK_NEXT_TOKEN(res, TOKEN_IMPLICATION, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "D");

    assert(list_token_empty(res));
    free(res);
}

TEST(from_ast_input_two) {
    char *str = "P1'->!QQ->!R10&S|!T&U&V";
    struct list_token **res = tokenize(str);

    print_token_list(res);
    printf("\n");

    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "P1'");
    CHECK_NEXT_TOKEN(res, TOKEN_IMPLICATION, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_NOT, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "QQ");
    CHECK_NEXT_TOKEN(res, TOKEN_IMPLICATION, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_NOT, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "R10");
    CHECK_NEXT_TOKEN(res, TOKEN_AND, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "S");
    CHECK_NEXT_TOKEN(res, TOKEN_OR, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_NOT, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "T");
    CHECK_NEXT_TOKEN(res, TOKEN_AND, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "U");
    CHECK_NEXT_TOKEN(res, TOKEN_AND, NULL);
    CHECK_NEXT_TOKEN(res, TOKEN_VARIABLE, "V");

    assert(list_token_empty(res));
    free(res);
}

int main() {
    #ifndef DISABLED
    RUN_TEST(simple);
    RUN_TEST(from_ast);
    RUN_TEST(from_ast_priority);
    RUN_TEST(from_ast_input_two);
    #else
    printf("TEST DISABLED\n");
    #endif
    return 0;
}