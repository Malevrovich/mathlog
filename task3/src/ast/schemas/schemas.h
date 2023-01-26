#pragma once

#ifndef _SCHEMAS_H_
#define _SCHEMAS_H_

#include "ast.h"
#include "list.h"

DECLARE_LIST(ast, struct AST*)

void add_classic_axioms(struct list_ast **axiomas);

#endif /* _SCHEMAS_H_ */