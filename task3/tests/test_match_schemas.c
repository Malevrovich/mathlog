#include "test_utils.h"

#include "schemas.h"
#include "match_schemas.h"
#include "ast.h"
#include "ast_parse.h"

#include <assert.h>

// #define DISABLED

DEFINE_LIST(ast, struct AST *)

#define INIT_AXIOMAS                                           \
struct list_ast **axioms = malloc(sizeof(struct list_ast *));  \
*axioms = NULL;                                                \
add_classic_axioms(axioms)         

#define CLEAR list_ast_deep_free(axioms, free_ast)

TEST(simple) {
    INIT_AXIOMAS;
    struct AST *axiom = list_ast_front(axioms);
    
    struct AST *expr1 = parse_ast_from("(A -> (B -> E) -> C | !D) -> !PKD1''231 -> (A -> (B -> E) -> C | !D)").val;
    struct AST *expr2 = parse_ast_from("(A -> (B -> E) -> C | !D) -> !PKD1231 -> (!A -> (B -> E) -> C | !D)").val;
    struct AST *expr3 = parse_ast_from("!A&C -> (A|U|A|A|A|A|A) -> !A&C").val;
    struct AST *expr4 = parse_ast_from("!A&C -> (A|A|A|A|A|U) -> !A&B").val;
    struct AST *expr5 = parse_ast_from("(B -> (B -> B)) -> (A -> (B -> (B -> B)))").val;

    assert(is_ast_match(axiom, expr1));
    assert(!is_ast_match(axiom, expr2));
    assert(is_ast_match(axiom, expr3));
    assert(!is_ast_match(axiom, expr4));
    assert(is_ast_match(axiom, expr5));

    deep_free_ast(expr1);
    deep_free_ast(expr2);
    deep_free_ast(expr3);
    deep_free_ast(expr4);
    deep_free_ast(expr5);
    CLEAR;
}

TEST(second) {
    INIT_AXIOMAS;
    struct AST* axiom = list_ast_head(axioms)->next->val;

    struct AST *expr1 = parse_ast_from("(A->(A&B|C->!D))->(A->(A&B|C->!D)->!(!U&!(A|E)))->(A->!(!U&!(A|E)))").val;
    struct AST *expr2 = parse_ast_from("(A -> (A -> A)) -> ((A -> ((A -> A) -> A)) -> (A -> A))").val;
    struct AST *expr3 = parse_ast_from("(A -> (A -> A)) -> ((A -> ((A -> !A) -> !A)) -> (A -> A))").val;
    struct AST *expr4 = parse_ast_from("(A -> (A -> A)) -> ((A -> ((A -> !A) -> A)) -> (A -> A))").val;

    assert(is_ast_match(axiom, expr1));
    assert(is_ast_match(axiom, expr2));
    assert(!is_ast_match(axiom, expr3));
    assert(!is_ast_match(axiom, expr4));

    deep_free_ast(expr1);
    deep_free_ast(expr2);
    deep_free_ast(expr3);
    deep_free_ast(expr4);

    CLEAR;
}

int main() {
    #ifndef DISABLED
    RUN_TEST(simple);
    RUN_TEST(second);
    #else 
    printf("TEST DISABLED\n");
    #endif
}