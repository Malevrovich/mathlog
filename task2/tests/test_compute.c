#include "test_utils.h"

#include "ast.h"
#include "ast_parse.h"
#include "tokenize.h"
#include "compute.h"

#include <assert.h>
#include <stdint.h>

// #define DISABLED

DEFINE_LIST(token, struct token)

#define INIT_AST(ast, str)                        \
struct list_token **tokens = tokenize(str);       \
struct AST *ast = parse_ast(tokens).val

#define CHECK_COMPUTE(ast, expected_true, expected_false)   \
struct compute_res res = compute(ast);                      \
assert(res.true_cnt == expected_true);                      \
assert(res.false_cnt == expected_false)

#define CLEAR do{ list_token_free(tokens); free(tokens); deep_free_ast(ast); } while(0)

TEST(simple) {
    INIT_AST(ast, "A&B");
    CHECK_COMPUTE(ast, 1, 3);
    CLEAR;
}

TEST(input_one) {
    INIT_AST(ast, "A&!A");
    CHECK_COMPUTE(ast, 0, 2);
    CLEAR;
}

TEST(input_two) {
    INIT_AST(ast, "A->!B123");
    CHECK_COMPUTE(ast, 3, 1);
    CLEAR;
}

TEST(input_three){
    INIT_AST(ast, "((PPP->PPP')->PPP)->PPP");
    CHECK_COMPUTE(ast, 4, 0);
    CLEAR;
}

TEST(big) {
    INIT_AST(ast, "(A1&A2&A3&A4&A5&A6) | !(A7&A8&A9&A10&A11&A12) & (A13|A14|A15|A16)");
    struct compute_res res = compute(ast);
    fprintf(stderr, "OK %u\n", res.true_cnt);
    fprintf(stderr, "FALSE %u\n", res.false_cnt);
    CLEAR;
}

int main() {
    #ifndef DISABLED
    RUN_TEST(simple);
    RUN_TEST(input_one);
    RUN_TEST(input_two);
    RUN_TEST(input_three);
    RUN_TEST(big);
    #else
    printf("TEST DISABLED\n");
    #endif
}