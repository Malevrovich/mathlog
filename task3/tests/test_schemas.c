#include "test_utils.h"

#include "schemas.h"
#include "list.h"
#include "ast_debug.h"

#include <assert.h>

#define DISABLED

DEFINE_READ_LIST(ast, struct AST*)

TEST(classic) {
    struct list_ast **axioms = malloc(sizeof(struct list_ast *));
    *axioms = NULL;

    add_classic_axioms(axioms);

    struct list_ast *head = list_ast_head(axioms);
    struct list_ast *cur = head;

    size_t idx = 0;
    printf("(%zu) ", idx);
    print_ast(stdout, cur->val);
    printf("\n");
    cur = cur->next;
    idx++;
    while(cur != head) {
        printf("(%zu) ", idx);
        print_ast(stdout, cur->val);
        printf("\n");
        cur = cur->next;
        idx++;
    }

    assert(idx == 10);
}

int main() {
    #ifndef DISABLED
    RUN_TEST(classic);
    #else
    printf("TEST DISABLED\n");
    #endif
}