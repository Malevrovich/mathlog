#pragma once

#ifndef _DEDUCTION_PARSE_H_
#define _DEDUCTION_PARSE_H_

#include "ast.h"
#include "list.h"

#include <stdio.h>

DECLARE_LIST(ast, struct AST *)

struct deduction_header {
    struct list_ast **hypothesis;
    struct AST *goal;
};

static const struct deduction_header INVALID_DEDUCTION_HEADER = (struct deduction_header) { NULL, NULL };

struct deduction_header read_deduction_header(FILE * restrict in);
struct deduction_header parse_deduction_header(char * restrict str);

void free_deduction_header(struct deduction_header *header);

#endif /* _DEDUCTION_PARSE_H_ */