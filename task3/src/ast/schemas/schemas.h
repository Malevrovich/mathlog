/**
 * @file schemas.h
 * @brief Sets of axiom's schemas
 */
#pragma once

#ifndef _SCHEMAS_H_
#define _SCHEMAS_H_

#include "ast.h"
#include "ast_list.h"

/**
 * @brief Adds hilbert axiom's schemas to AST list
 * 
 * @param [out] axiomas Where to add schemas 
 */
void add_classic_axioms(struct list_ast **axiomas);

#endif /* _SCHEMAS_H_ */