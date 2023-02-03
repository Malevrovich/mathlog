/**
 * @file ast_index.h
 * @brief Gives unique number for every variable in expression
 */
#pragma once

#ifndef _AST_INDEX_H_
#define _AST_INDEX_H_

#include "ast.h"

/**
 * @brief Gives unique number for every variable in expression
 * 
 * Recursively goes through the tree and mark every variable with number from 0 to N-1, \n 
 * where N is the number of variables
 * 
 * @param [in] root Tree that will be indexed 
 * @return size_t N - Number of variables
 */
size_t index(struct AST *root);

#endif /* _AST_INDEX_H_ */