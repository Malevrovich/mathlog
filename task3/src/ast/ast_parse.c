#include "ast.h"
#include "ast_build.h"
#include "ast_parse.h"
#include "token_ast_convert.h"
#include "tokenize.h"
#include "ast_debug.h"
#include "ast_list.h"

#include <stdlib.h>
#include <stdio.h>

DEFINE_LIST(token, struct token)

DEFINE_LIST(ast, struct AST *)

const char *PARSE_AST_RES_STRING[] = {
    [PARSE_AST_SUCCESS] = "Success!",
    [PARSE_AST_TOKEN_ERR] = "Tokenization error!",
    [PARSE_AST_INVALID_SEQUENCE] = "Invalid sequence!",
    [PARSE_AST_NULL_POINTER] = "Null pointer!",
    [PARSE_AST_UNKNOWN_TOKEN] = "Unknown token!"
};

static const short OP_PRIORITY[] = {
    [BIN_AND] = 3,
    [BIN_OR] = 2,
    [BIN_IMPLICATION] = 1
};

enum associativity {
    LEFT_ASSOCIATIVE, RIGHT_ASSOCIATIVE
};

const enum associativity OP_ASSOCIATIVITY[] = {
    [BIN_AND] = LEFT_ASSOCIATIVE,
    [BIN_OR] = LEFT_ASSOCIATIVE,
    [BIN_IMPLICATION] = RIGHT_ASSOCIATIVE
};

enum parse_res_status_internal {
    PARSE_AST_CONTINUE = PARSE_AST_UNKNOWN_TOKEN + 1, PARSE_AST_BREAK
};

static const struct parse_ast_res PARSE_CONTINUE = (struct parse_ast_res ) { NULL, (int) PARSE_AST_CONTINUE };
static const struct parse_ast_res PARSE_BREAK = (struct parse_ast_res) { NULL, (int) PARSE_AST_BREAK };

static struct parse_ast_res with_free_resources(struct list_ast **res, struct list_token **buf,
                                                                         struct parse_ast_res parse_res) {
    list_ast_deep_free(res, &free_ast);
    list_token_free(buf);
    free(res);
    free(buf);
    return parse_res;
}

static bool put_to_res(struct list_ast **res, struct AST *node) {
    if(node->type == AST_LITERAL || node->type == AST_NONE) {
        list_ast_push_back(res, node);
        return true;
    }

    if(list_ast_empty(res)) return false;
    struct AST *arg1 = list_ast_pop_back(res);
    
    if(node->type == AST_UNARY) {
        node->as_un.operand = arg1;
        list_ast_push_back(res, node);
        return true;
    }

    if(list_ast_empty(res)) return false;
    struct AST *arg2 = list_ast_pop_back(res);
    
    node->as_bin.lhs = arg2;
    node->as_bin.rhs = arg1;

    list_ast_push_back(res, node);
    return true;
}

static struct parse_ast_res parse_none(struct list_ast **res, struct list_token **buf, struct token cur) {
    if(cur.type == TOKEN_END) return PARSE_BREAK;
    if(cur.type == TOKEN_ERROR) return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);        
    if(cur.type == TOKEN_L_PARENTHESIS) { 
        list_token_push_back(buf, cur);
        return PARSE_CONTINUE;
    }
    if(cur.type == TOKEN_R_PARENTHESIS) {
        while(!list_token_empty(buf)) {
            struct token tmp = list_token_pop_back(buf);
            if(tmp.type == TOKEN_L_PARENTHESIS) break;

            struct AST *node = node_from_token(tmp);
            
            if(!node) return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);
            if(!put_to_res(res, node)) return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);
        }
        return PARSE_CONTINUE;
    }    
    return with_free_resources(res, buf, PARSE_UNKNOWN_TOKEN);
}

static struct parse_ast_res parse_op(struct list_ast **res, struct list_token **buf, struct token cur) {
    if(list_token_empty(buf)) {
        list_token_push_back(buf, cur);
        return PARSE_CONTINUE;
    }

    struct token top_token = list_token_back(buf);
    if(top_token.type == TOKEN_L_PARENTHESIS) {
        list_token_push_back(buf, cur);
        return PARSE_CONTINUE;
    }

    struct AST *node = node_from_token(cur);
    struct AST *top_node = node_from_token(top_token);

    if(top_node->type == AST_UNARY && node->type == AST_UNARY) {
        list_token_push_back(buf, cur);
        return PARSE_CONTINUE;
    }

    if(top_node->type == AST_UNARY && node->type == AST_BINARY) {
        if(!put_to_res(res, top_node)) return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);
        list_token_pop_back(buf);
        return parse_op(res, buf, cur);
    } 

    if(top_node->type == AST_BINARY && node->type == AST_UNARY) {
        list_token_push_back(buf, cur);
        return PARSE_CONTINUE;
    }

    if(!node || !top_node || (top_node->type != AST_BINARY && node->type != AST_BINARY)) 
        return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);

    if(OP_PRIORITY[top_node->as_bin.type] == OP_PRIORITY[node->as_bin.type]) {
        if(OP_ASSOCIATIVITY[node->as_bin.type] == LEFT_ASSOCIATIVE) {
            if(!put_to_res(res, top_node)) return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);
            list_token_pop_back(buf);
            return parse_op(res, buf, cur);
        }
    }

    if(OP_PRIORITY[top_node->as_bin.type] > OP_PRIORITY[node->as_bin.type]) {
        if(!put_to_res(res, top_node)) return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);
        list_token_pop_back(buf);
        return parse_op(res, buf, cur);
    }

    list_token_push_back(buf, cur);
    return PARSE_CONTINUE;
}

struct parse_ast_res parse_ast(struct list_token **tokens) {

    struct list_ast **res = (struct list_ast **) malloc(sizeof(struct list_ast *));
    struct list_token **buf = (struct list_token **) malloc(sizeof(struct list_buf *));

    if(!res || !buf) return (struct parse_ast_res){.val=NULL, .status=PARSE_AST_TOKEN_ERR};
    *res = NULL;
    *buf = NULL;

    while(!list_token_empty(tokens)) {
        struct token cur = list_token_front(tokens);
        enum AST_type ast_type = TOKENS_AST_TYPE[cur.type];

        if(ast_type == AST_NONE) {
            struct parse_ast_res parse_res = parse_none(res, buf, cur);
            if((int) parse_res.status != PARSE_AST_CONTINUE) return parse_res;
        } 
        else if(ast_type == AST_UNARY || ast_type == AST_BINARY) {
            struct parse_ast_res parse_res = parse_op(res, buf, cur);
            if((int)parse_res.status != PARSE_AST_CONTINUE) return parse_res;
        } 
        else if(ast_type == AST_LITERAL) {
            list_ast_push_back(res, node_from_token(cur));
        } 
        else {
            return with_free_resources(res, buf, PARSE_UNKNOWN_TOKEN);
        }

        list_token_pop_front(tokens);
    }

    while(!list_token_empty(buf)) {
        struct token tmp = list_token_pop_back(buf);
        struct AST *node = node_from_token(tmp);
        if(!node || !put_to_res(res, node)) return with_free_resources(res, buf, PARSE_UNKNOWN_TOKEN);
    }

    if(list_ast_empty(res)) return (struct parse_ast_res) { .val=NULL, .status=PARSE_AST_SUCCESS };
    if(list_ast_size(res) != 1) return with_free_resources(res, buf, PARSE_INVALID_SEQUENCE);

    struct AST *res_node = list_ast_pop_back(res);
    free(res);
    free(buf);
    return (struct parse_ast_res) { .val=res_node, .status=PARSE_AST_SUCCESS };
}

struct parse_ast_res parse_ast_from(const char * restrict str) {
    struct list_token **tokens = tokenize(str);

    if(!tokens) return (struct parse_ast_res) { .val=NULL, .status=PARSE_AST_TOKEN_ERR };
    
    struct parse_ast_res res = parse_ast(tokens);
    list_token_free(tokens);
    free(tokens);

    return res;
}
