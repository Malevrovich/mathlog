#pragma once

#ifndef _COMPUTE_H_
#define _COMPUTE_H_

#include "ast.h"

#include <stdint.h>

struct compute_res {
    uint32_t true_cnt;
    uint32_t false_cnt;
};

struct compute_res compute(struct AST *root);

#endif /* _COMPUTE_H_ */