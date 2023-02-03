/**
 * @file deduction_read.h
 * @brief Functions to read the deduction first line
 */
#pragma once

#ifndef _DEDUCTION_PARSE_H_
#define _DEDUCTION_PARSE_H_

#include "ast.h"
#include "ast_list.h"

#include <stdio.h>

/**
 * @brief Internal representation of deduction header: h_1, ..., h_n |- beta
 * 
 * Internal represenation of deduction header that contents: \n
 * List of hypotheses and beta represented in AST form
 */
struct deduction_header {
    struct list_ast **hypotheses;
    struct AST *goal;
};

static const struct deduction_header INVALID_DEDUCTION_HEADER = (struct deduction_header) { NULL, NULL };

/**
 * @brief Function to read deduction header from stream.
 * 
 * Reads the line from file and parse it to header. \n
 * Format of header: h_1, h_2, ..., h_n |- beta \n
 * Where: h_1, ..., h_n and beta - statements
 * 
 * @param [in] in File that contains header 
 * @return struct deduction_header
 */
struct deduction_header read_deduction_header(FILE * restrict in);
/**
 * @brief Function to parse deduction header from string.
 * 
 * Parse str to struct deduction_header. \n
 * Format of header: h_1, h_2, ..., h_n |- beta \n
 * Where: h_1, ..., h_n and beta - statements
 * 
 * @param [in] str String that contains header 
 * @return struct deduction_header
 */
struct deduction_header parse_deduction_header(char * restrict str);

void free_deduction_header(struct deduction_header *header);

#endif /* _DEDUCTION_PARSE_H_ */