#pragma once

#ifndef _TOKENIZE_DEBUG_H_
#define _TOKENIZE_DEBUG_H_

#include "list.h"
#include "tokenize.h"

extern const char *TOKEN_STRING[];

void print_token_list(struct list_token **list);

#endif /* _TOKENIZE_DEBUG_H_ */
