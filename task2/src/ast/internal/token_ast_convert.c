#include "ast.h"
#include "ast_build.h"
#include "token_ast_convert.h"

struct AST *node_from_token(struct token token) {
    struct AST *res = create_node();
    
    if(TOKENS_AST_TYPE[token.type] == AST_BINARY) *res = make_binop(TOKENS_OP_TYPE[token.type]);
    else if(TOKENS_AST_TYPE[token.type] == AST_NONE) *res = make_none();
    else if(TOKENS_AST_TYPE[token.type] == AST_UNARY) *res = make_unop(TOKENS_OP_TYPE[token.type]);
    else *res = make_var(token.value);
    
    return res;
}
