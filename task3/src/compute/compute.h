/**
 * @file compute.h
 * @brief Process the logical formula with all of possible sets of values and return's the result
 */
#pragma once

#ifndef _COMPUTE_H_
#define _COMPUTE_H_

#include "ast.h"

#include <stdint.h>

struct compute_res {
    uint32_t true_cnt;
    uint32_t false_cnt;
};

/**
 * @brief Computes the logical formula with every possible set of values and count number of true and false results
 * 
 * @param [in] root Logical formula to compute 
 * @return struct compute_res count of true and false results
 */
struct compute_res compute(struct AST *root);

#endif /* _COMPUTE_H_ */