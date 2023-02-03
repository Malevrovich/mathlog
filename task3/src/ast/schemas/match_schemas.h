/**
 * @file match_schemas.h
 * @brief Match formulas with schemas
 */
#pragma once

#ifndef _MATCH_SCHEMAS_H_
#define _MATCH_SCHEMAS_H_

#include "ast.h"

/**
 * @brief Check if formula match pattern/schema
 * 
 * @param [in] pattern Pattern in form of AST with special nodes 
 * @param [in] root Tree to check
 * @return true The formula could be generated with pattern
 * @return false The formula doesn't suit the pattern
 */
bool is_ast_match(struct AST *pattern, struct AST *root);

#endif