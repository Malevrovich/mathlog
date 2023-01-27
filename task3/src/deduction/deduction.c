#include "deduction.h"

#include "ast.h"
#include "ast_parse.h"
#include "ast_build.h"
#include "ast_debug.h"
#include "deduction_read.h"
#include "match_schemas.h"
#include "schemas.h"

#include <stdio.h>

#define BUFFER_SIZE (100 * 1024 + 1)

DEFINE_LIST(ast, struct AST*)

struct process_line_res {
    struct list_ast **out;
    enum {
        PROCESS_LINE_RES_ERROR, PROCESS_LINE_RES_CONTINUE, PROCESS_LINE_RES_END
    } status;
    enum {
        PROCESS_LINE_AXIOM_HYPOTHESIS, PROCESS_LINE_ALPHA, PROCESS_LINE_MODUS_PONENS
    } type;
};

static struct process_line_res process_hypothesis_or_axiom(struct AST *expr, struct AST *alpha) {
    struct list_ast **out = malloc(sizeof(struct list_ast *));
    *out = NULL;

    struct AST *first_root = build_node(make_binop(BIN_IMPLICATION));
    first_root->as_bin.rhs = build_node(make_binop(BIN_IMPLICATION));
    first_root->as_bin.lhs = expr;
    first_root->as_bin.rhs->as_bin.rhs = expr;
    first_root->as_bin.rhs->as_bin.lhs = alpha;

    struct AST *second_root = expr;

    struct AST *third_root = build_node(make_binop(BIN_IMPLICATION));
    third_root->as_bin.lhs = alpha;
    third_root->as_bin.rhs = expr;

    list_ast_push_back(out, first_root);
    list_ast_push_back(out, second_root);
    list_ast_push_back(out, third_root);

    return (struct process_line_res) {
        .out = out, .status = PROCESS_LINE_RES_CONTINUE, .type = PROCESS_LINE_AXIOM_HYPOTHESIS
    };
}
static struct process_line_res process_alpha(struct AST *alpha) {
    struct list_ast **out = malloc(sizeof(struct list_ast *));
    *out = NULL;

    /* A->(A->A) */
    struct AST *first_part = build_node(make_binop(BIN_IMPLICATION));
    first_part->as_bin.lhs = alpha;
    first_part->as_bin.rhs = build_node(make_binop(BIN_IMPLICATION));
    first_part->as_bin.rhs->as_bin.lhs = alpha;
    first_part->as_bin.rhs->as_bin.rhs = alpha;

    /* A->(A->A)->A */
    struct AST *second_part = build_node(make_binop(BIN_IMPLICATION));
    second_part->as_bin.lhs = alpha;
    second_part->as_bin.rhs = build_node(make_binop(BIN_IMPLICATION));
    second_part->as_bin.rhs->as_bin.rhs = alpha;
    second_part->as_bin.rhs->as_bin.lhs = build_node(make_binop(BIN_IMPLICATION));
    second_part->as_bin.rhs->as_bin.lhs->as_bin.lhs = alpha;
    second_part->as_bin.rhs->as_bin.lhs->as_bin.rhs = alpha;

    /* A->A */
    struct AST *third_part = build_node(make_binop(BIN_IMPLICATION));
    third_part->as_bin.lhs = alpha;
    third_part->as_bin.rhs = alpha;

    /* A->(A->A) */
    list_ast_push_back(out, first_part);

    /* (A->(A->A))->(A->(A->A)->A)->(A->A) */
    struct AST *second_root = build_node(make_binop(BIN_IMPLICATION));
    second_root->as_bin.rhs = build_node(make_binop(BIN_IMPLICATION));
    second_root->as_bin.rhs->as_bin.lhs = second_part;
    second_root->as_bin.rhs->as_bin.rhs = third_part;
    second_root->as_bin.lhs = first_part;
    list_ast_push_back(out, second_root);

    /* (A->(A->A)->A)->(A->A) */
    struct AST *third_root = build_node(make_binop(BIN_IMPLICATION));
    third_root->as_bin.lhs = second_part;
    third_root->as_bin.rhs = third_part;
    list_ast_push_back(out, third_root);

    /* A->(A->A)->A */
    list_ast_push_back(out, second_part);

    /* A->A */
    list_ast_push_back(out, third_part);

    return (struct process_line_res) {
        .out = out, .status = PROCESS_LINE_RES_CONTINUE, .type=PROCESS_LINE_ALPHA
    };  
}

static struct process_line_res process_modus_ponens(struct AST *expr, struct AST *A, struct AST *B, struct AST *alpha) {
    /* A = delta_j and B = delta_j -> expr */
    struct list_ast **out = malloc(sizeof(struct list_ast *));
    *out = NULL;

    /* (a -> delta_j) */
    struct AST *first_part = build_node(make_binop(BIN_IMPLICATION));
    first_part->as_bin.lhs = alpha;
    first_part->as_bin.rhs = A;

    /* (a -> delta-j -> expr) -> (a -> expr) */
    struct AST *second_part = build_node(make_binop(BIN_IMPLICATION));
    second_part->as_bin.rhs = build_node(make_binop(BIN_IMPLICATION));
    second_part->as_bin.rhs->as_bin.lhs = alpha;
    second_part->as_bin.rhs->as_bin.rhs = expr;

    second_part->as_bin.lhs = build_node(make_binop(BIN_IMPLICATION));
    second_part->as_bin.lhs->as_bin.lhs = alpha;
    second_part->as_bin.lhs->as_bin.rhs = B;

    struct AST *first_root = build_node(make_binop(BIN_IMPLICATION));
    first_root->as_bin.lhs = first_part;
    first_root->as_bin.rhs = second_part;

    list_ast_push_back(out, first_root);
    list_ast_push_back(out, second_part);

    struct AST *third_root = build_node(make_binop(BIN_IMPLICATION));
    third_root->as_bin.lhs = alpha;
    third_root->as_bin.rhs = expr;

    list_ast_push_back(out, third_root);

    return (struct process_line_res) {
        .out = out, .status = PROCESS_LINE_RES_CONTINUE, .type=PROCESS_LINE_MODUS_PONENS
    };  
}

static void free_res(struct process_line_res res) {
    if(res.status == PROCESS_LINE_RES_ERROR) return;
    if(!res.out) return;
    if(res.type == PROCESS_LINE_ALPHA) {
        struct AST *third_part = list_ast_back(res.out);
        free(third_part);
        list_ast_pop_back(res.out);

        struct AST *second_part = list_ast_back(res.out);
        free(second_part->as_bin.rhs->as_bin.lhs);
        free(second_part->as_bin.rhs);
        free(second_part);
        list_ast_pop_back(res.out);

        struct AST *third_root = list_ast_back(res.out);
        free(third_root);
        list_ast_pop_back(res.out);

        struct AST *second_root = list_ast_back(res.out);
        free(second_root->as_bin.rhs);
        free(second_root);
        list_ast_pop_back(res.out);

        struct AST *first_part = list_ast_back(res.out);
        free(first_part->as_bin.rhs);
        free(first_part);
        list_ast_pop_back(res.out);
    } else if(res.type == PROCESS_LINE_AXIOM_HYPOTHESIS) {
        struct AST *third_root = list_ast_back(res.out);
        free(third_root);
        list_ast_pop_back(res.out);

        list_ast_pop_back(res.out);

        struct AST *first_root = list_ast_back(res.out);
        free(first_root->as_bin.rhs);
        free(first_root);
        list_ast_pop_back(res.out);
    } else {
        struct AST *third_root = list_ast_back(res.out);
        free(third_root);
        list_ast_pop_back(res.out);

        struct AST *second_part = list_ast_back(res.out);
        free(second_part->as_bin.rhs);
        free(second_part->as_bin.lhs);
        free(second_part);
        list_ast_pop_back(res.out);

        struct AST *first_root = list_ast_back(res.out);
        free(first_root->as_bin.lhs);
        free(first_root);
        list_ast_pop_back(res.out);
    }

    list_ast_free(res.out);
    free(res.out);
}

static inline bool ast_modus_ponens_B_eq(struct AST *list_el, struct AST *expr) {
    return (list_el->type == AST_BINARY) && (list_el->as_bin.type == BIN_IMPLICATION) &&
            is_ast_equal(expr, list_el->as_bin.rhs);
}

static bool find_modus_ponens(struct list_ast **history, struct AST *expr, struct AST **A, struct AST **B) {
    /* Finds A and B: A = a and B = a -> expr */
    struct list_ast *head = list_ast_head(history);
    struct list_ast *cur = head;

    do{
        if(ast_modus_ponens_B_eq(cur->val, expr)) {
            struct list_ast *B_stop = cur;
            *B = cur->val;
            do {
                cur = cur->prev;
            } while(cur != head->prev && !is_ast_equal(cur->val, (*B)->as_bin.lhs));

            if(cur != head->prev) {
                *A = cur->val;
                return true;
            }
            cur = B_stop;
            do {
                cur = cur->next;
            } while(cur != head && !is_ast_equal(cur->val, (*B)->as_bin.lhs));
            if(cur != head) {
                *A = cur->val;
                return true;
            }
            cur = B_stop;
            *B = NULL;
        }
        cur = cur->next;
    } while(cur != head);
    return false;
}

static struct process_line_res process_expr(struct AST *expr, struct list_ast **history,
                                            struct deduction_header header, struct list_ast **axioms) {
    /* CHECK IF HYPOTHESIS */
    struct list_ast *hypot = list_ast_find(header.hypotheses, expr, is_ast_equal);
    if(hypot) {
        if(hypot->next == list_ast_head(header.hypotheses)) {
            return process_alpha(expr);
        }
        return process_hypothesis_or_axiom(expr, list_ast_back(header.hypotheses));
    }

    /* CHECK IF AXIOM */
    struct list_ast *axiom_head = list_ast_head(axioms);
    struct list_ast *axiom = axiom_head;
    do {
        if(is_ast_match(axiom->val, expr)) {
            return process_hypothesis_or_axiom(expr, list_ast_back(header.hypotheses));
        }
        axiom = axiom->next;
    } while(axiom != axiom_head);

    /* PROOF IS CORRECT, SO IT'S MODUS PONENS */
    /* FIND MODUS PONENS PARTS */
    struct AST *modus_ponens_a = NULL;
    struct AST *modus_ponens_b = NULL;
    
    if(!find_modus_ponens(history, expr, &modus_ponens_a, &modus_ponens_b)) {
        return (struct process_line_res) { .out=NULL, .status=PROCESS_LINE_RES_ERROR };
    }
    return process_modus_ponens(expr, modus_ponens_a, modus_ponens_b, list_ast_back(header.hypotheses));
}

static struct process_line_res process_line(char *str, struct list_ast **history,
                                            struct deduction_header header, struct list_ast **axioms) {
    if(!str) return (struct process_line_res) { .out=NULL, .status=PROCESS_LINE_RES_ERROR };

    struct parse_ast_res expr_parse_res = parse_ast_from(str);
    if(expr_parse_res.status != PARSE_AST_SUCCESS) 
        return (struct process_line_res) { .out=NULL, .status=PROCESS_LINE_RES_ERROR };
    
    if(!expr_parse_res.val) return (struct process_line_res) {.out=NULL, .status=PROCESS_LINE_RES_CONTINUE};
    
    struct process_line_res res = process_expr(expr_parse_res.val, history, header, axioms);    
    if(res.status == PROCESS_LINE_RES_ERROR) {
        deep_free_ast(expr_parse_res.val);
        return res;
    }
    list_ast_push_back(history, expr_parse_res.val);

    if(is_ast_equal(expr_parse_res.val, header.goal)) res.status = PROCESS_LINE_RES_END;
    
    return res;
}

void process_proof(FILE *in, FILE *out) {
    struct deduction_header header = read_deduction_header(in);

    if(!header.goal || !header.hypotheses) {
        fprintf(out, "Wrong deduction header!\n");
        return;
    }

    struct list_ast *proof_hypothesis = list_ast_head(header.hypotheses);
    while(proof_hypothesis != list_ast_last(header.hypotheses)) {
        print_ast(out, proof_hypothesis->val);
        if(proof_hypothesis->next != list_ast_last(header.hypotheses)) {
            fprintf(out, ",");
        }
        proof_hypothesis = proof_hypothesis->next;
    }

    struct AST *proof_header = build_node(make_binop(BIN_IMPLICATION));
    proof_header->as_bin.lhs = list_ast_back(header.hypotheses);
    proof_header->as_bin.rhs = header.goal;

    fprintf(out, "|-");
    print_ast(out, proof_header);
    fprintf(out, "\n");
    free(proof_header);

    struct list_ast **axioms = malloc(sizeof(struct list_ast *));
    *axioms = NULL;

    add_classic_axioms(axioms);
    
    struct list_ast **history = malloc(sizeof(struct list_ast *));
    *history = NULL;

    char *buffer = calloc(BUFFER_SIZE, sizeof(char));

    struct process_line_res line_res;
    do {
        if(!fgets(buffer, BUFFER_SIZE, in)) {
            fprintf(out, "UNABLE TO READ PROOF LINE\n");
            break;;
        }
        
        line_res = process_line(buffer, history, header, axioms);
        if(line_res.status == PROCESS_LINE_RES_ERROR) {
            fprintf(out, "PROCESS LINE ERROR\n");
            break;
        }
        
        if(!list_ast_empty(line_res.out)) {
            struct list_ast *head = list_ast_head(line_res.out);
            struct list_ast *cur = head;
            do{
                print_ast(out, cur->val);
                fprintf(out, "\n");
                cur = cur->next;
            } while(cur != head);
            
            free_res(line_res);
        }
    } while(line_res.status == PROCESS_LINE_RES_CONTINUE);

    list_ast_deep_free(axioms, free_ast);
    list_ast_deep_free(history, free_ast);
    free(axioms);
    free(history);
    free_deduction_header(&header);
}