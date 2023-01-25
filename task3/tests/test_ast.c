#include "test_utils.h"
#include "ast.h"
#include "ast_debug.h"
#include "ast_index.h"
#include "ast_parse.h"
#include "tokenize.h"
#include "token_ast_convert.h"

#include <assert.h>
#include <string.h>

// #define DISABLED

DEFINE_LIST(token, struct token)

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

#define CLEAR do{ list_token_free(tokens); free(tokens); deep_free_ast(ast); } while(0)

TEST(convert) {
    char *str = "|";
    struct list_token **tokens = tokenize(str);

    struct token tok = list_token_pop_back(tokens);
    struct AST *ast = node_from_token(tok);

    assert(ast->type == AST_BINARY);
    assert(ast->as_bin.type == BIN_OR);

    list_token_free(tokens);
    free(tokens);
    free(ast);
}

TEST(priority) {
    char *str = "A & B | C -> D";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST *ast = parse_res.val;
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    CHECK_BIN(ast, BIN_IMPLICATION);
    
    CHECK_BIN(ast->as_bin.lhs, BIN_OR);
    CHECK_LITERAL(ast->as_bin.rhs, "D");

    CHECK_BIN(ast->as_bin.lhs->as_bin.lhs, BIN_AND);
    CHECK_LITERAL(ast->as_bin.lhs->as_bin.rhs, "C");

    CHECK_LITERAL(ast->as_bin.lhs->as_bin.lhs->as_bin.lhs, "A");
    CHECK_LITERAL(ast->as_bin.lhs->as_bin.lhs->as_bin.rhs, "B");

    CLEAR;
}

TEST(simple) {
    char *str = "D -> C | B & A";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST *ast = parse_res.val;
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    CHECK_BIN(ast, BIN_IMPLICATION);
    
    CHECK_LITERAL(ast->as_bin.lhs, "D");
    CHECK_BIN(ast->as_bin.rhs, BIN_OR);

    CHECK_LITERAL(ast->as_bin.rhs->as_bin.lhs, "C");
    CHECK_BIN(ast->as_bin.rhs->as_bin.rhs, BIN_AND);

    CHECK_LITERAL(ast->as_bin.rhs->as_bin.rhs->as_bin.lhs, "B");
    CHECK_LITERAL(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs, "A");

    CLEAR;
}

TEST(not) {
    char *str = "D -> C | !B & A";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST *ast = parse_res.val;
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    CHECK_BIN(ast, BIN_IMPLICATION);
    
    CHECK_LITERAL(ast->as_bin.lhs, "D");
    CHECK_BIN(ast->as_bin.rhs, BIN_OR);

    CHECK_LITERAL(ast->as_bin.rhs->as_bin.lhs, "C");
    CHECK_BIN(ast->as_bin.rhs->as_bin.rhs, BIN_AND);

    CHECK_UN(ast->as_bin.rhs->as_bin.rhs->as_bin.lhs, UN_NOT);
    CHECK_LITERAL(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs, "A");

    CHECK_LITERAL(ast->as_bin.rhs->as_bin.rhs->as_bin.lhs->as_un.operand, "B");
    CLEAR;
}

TEST(not_not) {
    char *str = "!!!A";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST* ast = parse_res.val;
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    CHECK_UN(ast, UN_NOT);
    CHECK_UN(ast->as_un.operand, UN_NOT);
    CHECK_UN(ast->as_un.operand->as_un.operand, UN_NOT);
    CHECK_LITERAL(ast->as_un.operand->as_un.operand->as_un.operand, "A");
    CLEAR;
}

TEST(impl_impl) {
    char *str = "A -> B -> C";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST* ast = parse_res.val;
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    CHECK_BIN(ast, BIN_IMPLICATION);
    CHECK_BIN(ast->as_bin.rhs, BIN_IMPLICATION);
    CLEAR;
}

TEST(parenthesis) {
    char *str = "!(!!A | !(B)) & (((A)) -> C)";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST* ast = parse_res.val;

    size_t max_idx = index(ast);
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    assert(max_idx == 3);
    CHECK_BIN(ast, BIN_AND);
    
    CHECK_UN(ast->as_bin.lhs, UN_NOT);
    CHECK_BIN(ast->as_bin.lhs->as_un.operand, BIN_OR);
    CHECK_UN(ast->as_bin.lhs->as_un.operand->as_bin.lhs, UN_NOT);
    CHECK_UN(ast->as_bin.lhs->as_un.operand->as_bin.lhs->as_un.operand, UN_NOT);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_un.operand->as_bin.lhs->as_un.operand->as_un.operand, "A", 0);

    CHECK_UN(ast->as_bin.lhs->as_un.operand->as_bin.rhs, UN_NOT);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_un.operand->as_bin.rhs->as_un.operand, "B", 1);

    CHECK_BIN(ast->as_bin.rhs, BIN_IMPLICATION);
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.lhs, "A", 0);
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.rhs, "C", 2);
    CLEAR;
}

TEST(input_one) {
    char *str = "!A&!B->!(A|B)";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST* ast = parse_res.val;
    size_t max_idx = index(ast);

    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    assert(max_idx == 2);
    CHECK_BIN(ast, BIN_IMPLICATION);
    
    CHECK_BIN(ast->as_bin.lhs, BIN_AND);
    
    CHECK_UN(ast->as_bin.lhs->as_bin.lhs, UN_NOT);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_bin.lhs->as_un.operand, "A", 0);

    CHECK_UN(ast->as_bin.lhs->as_bin.rhs, UN_NOT);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_bin.rhs->as_un.operand, "B", 1);

    CHECK_UN(ast->as_bin.rhs, UN_NOT);
    CHECK_BIN(ast->as_bin.rhs->as_un.operand, BIN_OR);
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_un.operand->as_bin.lhs, "A", 0);
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_un.operand->as_bin.rhs, "B", 1);
    CLEAR;
}

TEST(input_two) {
    char *str = "P1'->!QQ->!R10&S|!T&U&V";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST* ast = parse_res.val;
    size_t max_idx = index(ast);
    
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    assert(max_idx == 7);
    CHECK_BIN(ast, BIN_IMPLICATION);

    CHECK_LITERAL_IDX(ast->as_bin.lhs, "P1'", 0);
    
    CHECK_BIN(ast->as_bin.rhs, BIN_IMPLICATION);

    CHECK_UN(ast->as_bin.rhs->as_bin.lhs, UN_NOT);
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.lhs->as_un.operand, "QQ", 1);

    CHECK_BIN(ast->as_bin.rhs->as_bin.rhs, BIN_OR);
    CHECK_BIN(ast->as_bin.rhs->as_bin.rhs->as_bin.lhs, BIN_AND);
    
    CHECK_UN(ast->as_bin.rhs->as_bin.rhs->as_bin.lhs->as_bin.lhs, UN_NOT);
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.rhs->as_bin.lhs->as_bin.lhs->as_un.operand, "R10", 2);
    
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.rhs->as_bin.lhs->as_bin.rhs, "S", 3);

    CHECK_BIN(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs, BIN_AND);
    
    CHECK_BIN(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs->as_bin.lhs, BIN_AND);
    
    CHECK_UN(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs->as_bin.lhs->as_bin.lhs, UN_NOT);
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs->as_bin.lhs->as_bin.lhs->as_un.operand, "T", 4);
    
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs->as_bin.lhs->as_bin.rhs, "U", 5);
    
    CHECK_LITERAL_IDX(ast->as_bin.rhs->as_bin.rhs->as_bin.rhs->as_bin.rhs, "V", 6);

    CLEAR;
}

TEST(and_and) {
    char *str = "A&B&C";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST* ast = parse_res.val;
    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    CHECK_BIN(ast, BIN_AND);
    CHECK_BIN(ast->as_bin.lhs, BIN_AND);
    CHECK_LITERAL(ast->as_bin.lhs->as_bin.lhs, "A");
    CHECK_LITERAL(ast->as_bin.lhs->as_bin.rhs, "B");
    CHECK_LITERAL(ast->as_bin.rhs, "C");

    CLEAR;
}

TEST(index) {
    char *str = "A&B&C&A&B&D";
    struct list_token **tokens = tokenize(str);

    struct parse_ast_res parse_res = parse_ast(tokens);
    assert(parse_res.status == PARSE_AST_SUCCESS);

    struct AST* ast = parse_res.val;
    size_t max_ind = index(ast);

    print_ast(stderr, ast);
    fprintf(stderr, "\n");

    assert(max_ind == 4);
    CHECK_BIN(ast, BIN_AND);
    CHECK_LITERAL_IDX(ast->as_bin.rhs, "D", 3);
    CHECK_BIN(ast->as_bin.lhs, BIN_AND);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_bin.rhs, "B", 1);
    CHECK_BIN(ast->as_bin.lhs->as_bin.lhs, BIN_AND);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_bin.lhs->as_bin.rhs, "A", 0);
    CHECK_BIN(ast->as_bin.lhs->as_bin.lhs->as_bin.lhs, BIN_AND);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_bin.lhs->as_bin.lhs->as_bin.rhs, "C", 2);
    CHECK_BIN(ast->as_bin.lhs->as_bin.lhs->as_bin.lhs->as_bin.lhs, BIN_AND);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_bin.lhs->as_bin.lhs->as_bin.lhs->as_bin.rhs, "B", 1);
    CHECK_LITERAL_IDX(ast->as_bin.lhs->as_bin.lhs->as_bin.lhs->as_bin.lhs->as_bin.lhs, "A", 0);

    CLEAR;
}

int main() {
    #ifndef DISABLED
    RUN_TEST(convert);
    RUN_TEST(and_and);
    RUN_TEST(simple);
    RUN_TEST(priority);
    RUN_TEST(not);
    RUN_TEST(not_not);
    RUN_TEST(impl_impl);
    RUN_TEST(parenthesis);
    RUN_TEST(input_one);
    RUN_TEST(input_two);
    RUN_TEST(index);
    #else
    printf("TEST DISABLED\n");
    #endif
}