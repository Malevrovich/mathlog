#include "deduction_read.h"

#include "ast.h"
#include "ast_parse.h"
#include "list.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE (100 * 1024 + 1)

DEFINE_LIST(ast, struct AST*)

void free_deduction_header(struct deduction_header *header){
    deep_free_ast(header->goal);

    list_ast_deep_free(header->hypotheses, free_ast);
    free(header->hypotheses);
}

struct deduction_header read_deduction_header(FILE * restrict in) {
    char *buff = calloc(BUFFER_SIZE, sizeof(char));

    if(!buff) return INVALID_DEDUCTION_HEADER;

    if(!fgets(buff, BUFFER_SIZE, in)) {
        free(buff);
        return INVALID_DEDUCTION_HEADER;
    }

    struct deduction_header res = parse_deduction_header(buff);

    free(buff);
    
    return res;
}

struct deduction_header parse_deduction_header(char * restrict str) {
    char *goal = strstr(str, "|-");

    if(!goal) return INVALID_DEDUCTION_HEADER;

    goal[0] = '\0';
    goal[1] = '\0';
    goal += 2;

    struct deduction_header res;
    
    struct parse_ast_res goal_parse_res = parse_ast_from(goal);
    if(goal_parse_res.status != PARSE_AST_SUCCESS) return INVALID_DEDUCTION_HEADER;
    
    res.goal = goal_parse_res.val;

    res.hypotheses = (struct list_ast **) malloc(sizeof(struct list_ast *));
    *res.hypotheses = NULL;

    char *expr = strtok(str, ",");
    while(expr) {
        struct parse_ast_res expr_parse_res = parse_ast_from(expr);
        if(expr_parse_res.status != PARSE_AST_SUCCESS) {
            free_deduction_header(&res);

            return INVALID_DEDUCTION_HEADER;
        }

        list_ast_push_back(res.hypotheses, expr_parse_res.val);
        
        expr = strtok(NULL, ",");
    }

    return res;
}