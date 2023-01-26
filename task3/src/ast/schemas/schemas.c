#include "schemas.h"

#include "ast.h"
#include "ast_build.h"
#include "list.h"

DEFINE_LIST(ast, struct AST*)

#define IMPL build_node(make_binop(BIN_IMPLICATION))
#define PATTERN(idx) build_node(make_pattern(idx))

void add_classic_axioms(struct list_ast **axiomas) {
    do {
        /* (1) a -> b -> a */
        struct AST *root = IMPL;

        root->as_bin.lhs = PATTERN(0);
        root->as_bin.rhs = IMPL;
        root->as_bin.rhs->as_bin.lhs = PATTERN(1);
        root->as_bin.rhs->as_bin.rhs = PATTERN(0);

        list_ast_push_back(axiomas, root);
    } while(0);
    do{
        /* (2)  (a -> b) -> (a -> b -> c) -> (a -> c) */
        struct AST *root = IMPL;

        root->as_bin.lhs = IMPL;
        root->as_bin.lhs->as_bin.lhs = PATTERN(0);
        root->as_bin.lhs->as_bin.rhs = PATTERN(1);
        
        root->as_bin.rhs = IMPL;

        root->as_bin.rhs->as_bin.lhs = IMPL;
        root->as_bin.rhs->as_bin.lhs->as_bin.lhs = PATTERN(0);
        root->as_bin.rhs->as_bin.lhs->as_bin.rhs = IMPL;
        root->as_bin.rhs->as_bin.lhs->as_bin.rhs->as_bin.lhs = PATTERN(1);
        root->as_bin.rhs->as_bin.lhs->as_bin.rhs->as_bin.rhs = PATTERN(2);

        root->as_bin.rhs->as_bin.rhs = IMPL;
        root->as_bin.rhs->as_bin.rhs->as_bin.lhs = PATTERN(0);
        root->as_bin.rhs->as_bin.rhs->as_bin.rhs = PATTERN(2);

        list_ast_push_back(axiomas, root);
    } while(0);
    do {
        /* (3) a -> b -> a & b */
        struct AST *root = IMPL;
        root->as_bin.lhs = PATTERN(0);

        root->as_bin.rhs = IMPL;
        root->as_bin.rhs->as_bin.lhs = PATTERN(1);

        root->as_bin.rhs->as_bin.rhs = build_node(make_binop(BIN_AND));
        root->as_bin.rhs->as_bin.rhs->as_bin.lhs = PATTERN(0);
        root->as_bin.rhs->as_bin.rhs->as_bin.rhs = PATTERN(1);
        
        list_ast_push_back(axiomas, root);
    } while(0);
    do {
        /* (4) a & b -> a */
        struct AST *root = IMPL;

        root->as_bin.lhs = build_node(make_binop(BIN_AND));
        root->as_bin.lhs->as_bin.lhs = PATTERN(0);
        root->as_bin.lhs->as_bin.rhs = PATTERN(1);
        root->as_bin.rhs = PATTERN(0);

        list_ast_push_back(axiomas, root);
    } while (0);
    do {
        /* (5) a & b -> b */
        struct AST *root = IMPL;

        root->as_bin.lhs = build_node(make_binop(BIN_AND));
        root->as_bin.lhs->as_bin.lhs = PATTERN(0);
        root->as_bin.lhs->as_bin.rhs = PATTERN(1);
        root->as_bin.rhs = PATTERN(1);

        list_ast_push_back(axiomas, root);
    } while (0);
    do {
        /* (6) a -> a | b */
        struct AST *root = IMPL;

        root->as_bin.lhs = PATTERN(0);
        root->as_bin.rhs = build_node(make_binop(BIN_OR));
        root->as_bin.rhs->as_bin.lhs = PATTERN(0);
        root->as_bin.rhs->as_bin.rhs = PATTERN(1);

        list_ast_push_back(axiomas, root);
    } while (0);
    do {
        /* (7) b -> a | b */
        struct AST *root = IMPL;

        root->as_bin.lhs = PATTERN(1);
        root->as_bin.rhs = build_node(make_binop(BIN_OR));
        root->as_bin.rhs->as_bin.lhs = PATTERN(0);
        root->as_bin.rhs->as_bin.rhs = PATTERN(1);

        list_ast_push_back(axiomas, root);
    } while (0);
    do
    {
        /* (8) (a -> c) -> (b -> c) -> (a | b -> c) */
        struct AST *first = IMPL;
        first->as_bin.lhs = PATTERN(0);
        first->as_bin.rhs = PATTERN(2);

        struct AST *second = IMPL;
        second->as_bin.lhs = PATTERN(1);
        second->as_bin.rhs = PATTERN(2);

        struct AST *third = IMPL;
        third->as_bin.lhs = build_node(make_binop(BIN_OR));
        third->as_bin.lhs->as_bin.lhs = PATTERN(0);
        third->as_bin.lhs->as_bin.rhs = PATTERN(1);
        third->as_bin.rhs = PATTERN(2);

        struct AST *root = IMPL;
        root->as_bin.lhs = first;
        root->as_bin.rhs = IMPL;
        root->as_bin.rhs->as_bin.lhs = second;
        root->as_bin.rhs->as_bin.rhs = third;

        list_ast_push_back(axiomas, root);
    } while (0);
    do {
        /* (9) (a -> b) -> (a -> !b) -> !a */
        struct AST *first = IMPL;
        first->as_bin.lhs = PATTERN(0);
        first->as_bin.rhs = PATTERN(1);

        struct AST *second = IMPL;
        second->as_bin.lhs = PATTERN(0);
        second->as_bin.rhs = build_node(make_unop(UN_NOT));
        second->as_bin.rhs->as_un.operand = PATTERN(1);

        struct AST *third = build_node(make_unop(UN_NOT));
        third->as_un.operand = PATTERN(0);

        struct AST *root = IMPL;
        root->as_bin.lhs = first;
        root->as_bin.rhs = IMPL;
        root->as_bin.rhs->as_bin.lhs = second;
        root->as_bin.rhs->as_bin.rhs = third;

        list_ast_push_back(axiomas, root);
    } while(0);
    do {
        /* (10) !!a -> a */
        struct AST *root = IMPL;

        root->as_bin.lhs = build_node(make_unop(UN_NOT));
        root->as_bin.lhs->as_un.operand = build_node(make_unop(UN_NOT));
        root->as_bin.lhs->as_un.operand->as_un.operand = PATTERN(0);

        root->as_bin.rhs = PATTERN(0);

        list_ast_push_back(axiomas, root);
    } while(0);
}