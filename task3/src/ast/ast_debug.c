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

__attribute__((unused))
static void print_as_tree(FILE *out, struct AST* node) {
    if(!node) {
        fprintf(out, "(\nNULL\n)");
    } else if(node->type == AST_NONE) {
        fprintf(out, "(\nNONE\n)");
    } else if(node->type == AST_LITERAL) {
        fprintf(out, "(%s,%zu)", node->as_lit.value, node->as_lit.idx);
    } else if(node->type == AST_BINARY) {
        fprintf(out, "(%s,", BIN_OP_STRING[node->as_bin.type]);
        print_as_tree(out, node->as_bin.lhs);
        fprintf(out, ",");
        print_as_tree(out, node->as_bin.rhs);
        fprintf(out, ")");
    } else if(node->type == AST_UNARY) {
        fprintf(out, "(%s", UN_OP_STRING[node->as_un.type]);
        print_as_tree(out, node->as_un.operand);
        fprintf(out, ")");
    } else if(node->type == AST_PATTERN) {
        fprintf(out, "[%zu]", node->as_pattern.idx);
    } else {
        fprintf(out, "(\nUNKNOWN AST NODE\n)");
    }
}

static void print_natural(FILE *out, struct AST* node) {
    if(!node) {
        fprintf(out, "NULL");
    } else if(node->type == AST_NONE) {
        fprintf(out, "NONE");
    } else if(node->type == AST_LITERAL) {
        fprintf(out, "%s", node->as_lit.value);
    } else if(node->type == AST_PATTERN) {
        fprintf(out, "%zu", node->as_pattern.idx);
    } else if(node->type == AST_UNARY) {
        fprintf(out, "(%s", UN_OP_STRING[node->as_un.type]);
        print_natural(out, node->as_un.operand);
        fprintf(out, ")");
    } else if(node->type == AST_BINARY) {
        fprintf(out, "(");
        print_natural(out, node->as_bin.lhs);
        fprintf(out, " %s ", BIN_OP_STRING[node->as_bin.type]);
        print_natural(out, node->as_bin.rhs);
        fprintf(out, ")");
    } else {
        fprintf(out, "UNKNOWN");
    }
}

void snprint_natural(char *out, size_t n, struct AST* node) {
    if(!node) {
        snprintf(out, n, "NULL");
    } else if(node->type == AST_NONE) {
        snprintf(out, n, "NONE");
    } else if(node->type == AST_LITERAL) {
        snprintf(out, n, "%s", node->as_lit.value);
    } else if(node->type == AST_PATTERN) {
        snprintf(out, n, "%zu", node->as_pattern.idx);
    } else if(node->type == AST_UNARY) {
        snprintf(out, n, "(%s", UN_OP_STRING[node->as_un.type]);
        snprint_natural(out, n, node->as_un.operand);
        snprintf(out, n, ")");
    } else if(node->type == AST_BINARY) {
        snprintf(out, n, "(");
        snprint_natural(out, n, node->as_bin.lhs);
        snprintf(out, n, " %s ", BIN_OP_STRING[node->as_bin.type]);
        snprint_natural(out, n, node->as_bin.rhs);
        snprintf(out, n, ")");
    } else {
        snprintf(out, n, "UNKNOWN");
    }
}

void snprint_ast(char *out, size_t n, struct AST* node) {
    snprint_natural(out, n, node);
}

void print_ast(FILE *out, struct AST* node) {
    print_natural(out, node);    
}