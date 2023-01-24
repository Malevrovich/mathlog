#pragma once

#ifndef _AST_DEBUG_H_
#define _AST_DEBUG_H_

#include "ast.h"
#include <stdio.h>

extern const char *AST_TYPE_STRING[];
extern const char *BIN_OP_STRING[];
extern const char *UN_OP_STRING[];


void print_ast(FILE *out, struct AST* node);

#endif