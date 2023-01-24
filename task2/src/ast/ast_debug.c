#include "ast_debug.h"

#include <stdio.h>

const char *AST_TYPE_STRING[] = {
    [AST_NONE] = "AST_NONE",
    [AST_BINARY] = "AST_BINARY",
    [AST_LITERAL] = "AST_LITERAL",
    [AST_UNARY] = "AST_UNARY"
};

const char *BIN_OP_STRING[] = {
    [BIN_AND] = "&",
    [BIN_OR] = "|",
    [BIN_IMPLICATION] = "->"
};

const char *UN_OP_STRING[] = {
    [UN_NOT] = "!"
};

void print_ast(FILE *out, struct AST* node) {
    if(!node) {
        fprintf(out, "(\nNULL\n)");
    } else if(node->type == AST_NONE) {
        fprintf(out, "(\nNONE\n)");
    } else if(node->type == AST_LITERAL) {
        fprintf(out, "%s", node->as_lit.value);
    } else if(node->type == AST_BINARY) {
        fprintf(out, "(%s,", BIN_OP_STRING[node->as_bin.type]);
        print_ast(out, node->as_bin.lhs);
        fprintf(out, ",");
        print_ast(out, node->as_bin.rhs);
        fprintf(out, ")");
    } else if(node->type == AST_UNARY) {
        fprintf(out, "(%s", UN_OP_STRING[node->as_un.type]);
        print_ast(out, node->as_un.operand);
        fprintf(out, ")");
    } else {
        fprintf(out, "(\nUNKNOWN AST NODE\n)");
    }
}