#pragma once

#ifndef _MATCH_SCHEMAS_H_
#define _MATCH_SCHEMAS_H_

#include "ast.h"

bool is_match(struct AST *pattern, struct AST *root);

#endif