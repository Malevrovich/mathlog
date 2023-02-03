/**
 * @file deduction.h
 * @brief Proves the deduction theorem for a particular case
 */
#pragma once

#ifndef _DEDUCTION_H_
#define _DEDUCTION_H_

#include <stdio.h>

/**
 * @brief Generates the proof of h_1, ..., h_n |- alpha->beta based on proof of h_1, ..., h_n, alpha |- beta in Hilbert system
 * 
 * Proves the deduction theorem for a particular case in Hilbert system. \n
 * Generates the proof of statement alpha->beta in context of hypotheses h_1, ..., h_n. \n
 * Generated proof based on proof of statement beta in context of hypotheses h_1, ..., h_n, alpha. \n
 * Line by line goes through the input proof and checks the type of statement(let's define this statement as S): \n
 * If S is one of axioms, then it generates: \n
 * (1) S->alpha->S(as axiom's schema) \n
 * (2) S(as axiom's schema) \n
 * (3) alpha->S (as modus ponens of (1) and (2)) \n
 * If S is alpha, then it generates: \n
 * (1) alpha->(alpha->alpha) (as axiom's schema) \n
 * (2) (alpha->(alpha->alpha))->(alpha->(alpha->alpha)->alpha)->(alpha->alpha) (as axiom's schema) \n
 * (3) (alpha->(alpha->alpha)->alpha)->(alpha->alpha) (as modus ponens of (1) and (2)) \n
 * (4) alpha->(alpha->alpha)->alpha (as axiom's schema) \n
 * (5) alpha->alpha (as modus ponens of (3) and (4)) \n
 * If S  modus ponens of S_1 and S_1->S. \n
 * Proof of alpha->S_1 and alpha->S_1->S is already generated, so the function generates: \n
 * (1) (alpha->S_1)->(alpha->S_1->S)->(alpha->S) (as axiom's schema) \n
 * (2) (alpha->S_1->S)->(alpha->S) (as modus ponens of (alpha->S_1) and (1)) \n
 * (3) alpha->S (as modus ponens of (alpha->S_1->S) and (2))
 *
 * @param [in] in Input file, that contains h_1, ..., h_n, alpha |- beta proof
 * @param [out] out Output file, that will contain h_1, ..., h_n |- alpha->beta proof
 */
void process_proof(FILE *in, FILE *out);

#endif