/**
 * @file ast_debug.h
 * @brief Functions to debug AST
 */
#pragma once

#ifndef _AST_DEBUG_H_
#define _AST_DEBUG_H_

#include "ast.h"
#include <stdio.h>
/**
 * @brief String representation of enum AST_type
 */
extern const char *AST_TYPE_STRING[];
/**
 * @brief String representation of enum binop_type
 */
extern const char *BIN_OP_STRING[];
/**
 * @brief String representation of enum unop_type
 */
extern const char *UN_OP_STRING[];

/**
 * @brief Recursively prints AST
 * 
 * @param [out] out File to write 
 * @param [in] node Tree to print 
 */
void print_ast(FILE *out, struct AST* node);
/**
 * @brief Recursively prints AST to string, but up to N symbols
 * 
 * @param [out] out File to write
 * @param [in] n Maximum length 
 * @param [in] node Tree to print 
 */
void snprint_ast(char *out, size_t n, struct AST* node);

#endif