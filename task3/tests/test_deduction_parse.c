#include "test_utils.h"
#include "test_ast_toolkit.h"

#include "ast.h"
#include "deduction_read.h"

#include <assert.h>

#define DISABLED

#define PARSE_DEDUCTION_HEADER(string)                          \
char *str = calloc(100, sizeof(char));                          \
strcat(str, string);                                            \
struct deduction_header header = parse_deduction_header(str);   \
assert(header.goal != NULL);                                    \
assert(header.hypotheses != NULL)


#define INIT_AST_CHECK                                      \
assert(!list_ast_empty(header.hypotheses));                 \
struct list_ast *cur = list_ast_head(header.hypotheses);    \
struct list_ast *head = cur                                

#define NEXT cur = cur->next
#define CHECK_END assert(cur == head)
#define CLEAR free_deduction_header(&header); free(str);

DEFINE_READ_LIST(ast, struct AST *)

TEST(simple) {
    PARSE_DEDUCTION_HEADER("A|-B->B->B");    

    CHECK_BIN(header.goal, BIN_IMPLICATION);
    CHECK_BIN(header.goal->as_bin.rhs, BIN_IMPLICATION);
    CHECK_LITERAL(header.goal->as_bin.rhs->as_bin.lhs, "B");
    CHECK_LITERAL(header.goal->as_bin.rhs->as_bin.rhs, "B");
    CHECK_LITERAL(header.goal->as_bin.lhs, "B");

    INIT_AST_CHECK;

    CHECK_LITERAL(cur->val, "A");

    NEXT;                    
    CHECK_END;
    CLEAR;
}

TEST(two) {
    PARSE_DEDUCTION_HEADER("A, B |- A&B");

    CHECK_BIN(header.goal, BIN_AND);
    CHECK_LITERAL(header.goal->as_bin.lhs, "A");
    CHECK_LITERAL(header.goal->as_bin.rhs, "B");

    INIT_AST_CHECK;

    CHECK_LITERAL(cur->val, "A");

    NEXT;

    CHECK_LITERAL(cur->val, "B");

    NEXT;
    CHECK_END;
    CLEAR;
}

TEST(hard) {
    PARSE_DEDUCTION_HEADER("A->B, B|!(B->A), B->!A |-A|C");

    CHECK_BIN(header.goal, BIN_OR);
    CHECK_LITERAL(header.goal->as_bin.lhs, "A");
    CHECK_LITERAL(header.goal->as_bin.rhs, "C");

    INIT_AST_CHECK;

    CHECK_BIN(cur->val, BIN_IMPLICATION);
    CHECK_LITERAL(cur->val->as_bin.lhs, "A");
    CHECK_LITERAL(cur->val->as_bin.rhs, "B");

    NEXT;

    CHECK_BIN(cur->val, BIN_OR);
    CHECK_LITERAL(cur->val->as_bin.lhs, "B");
    CHECK_UN(cur->val->as_bin.rhs, UN_NOT);
    CHECK_BIN(cur->val->as_bin.rhs->as_un.operand, BIN_IMPLICATION);
    CHECK_LITERAL(cur->val->as_bin.rhs->as_un.operand->as_bin.lhs, "B");
    CHECK_LITERAL(cur->val->as_bin.rhs->as_un.operand->as_bin.rhs, "A");

    NEXT;

    CHECK_BIN(cur->val, BIN_IMPLICATION);
    CHECK_LITERAL(cur->val->as_bin.lhs, "B");
    CHECK_UN(cur->val->as_bin.rhs, UN_NOT);
    CHECK_LITERAL(cur->val->as_bin.rhs->as_un.operand, "A");

    NEXT;
    CHECK_END;
    CLEAR;
}

int main() {
    #ifndef DISABLED
    RUN_TEST(simple);
    RUN_TEST(two);
    RUN_TEST(hard);
    #else
    printf("TEST DISABLED\n");
    #endif
}