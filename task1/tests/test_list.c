#include "test_utils.h"
#include "list.h"

#include <assert.h>
#include <stdlib.h>

DEFINE_LIST(int, int)
DECLARE_LIST(int, int)

#define INIT_LIST(name) (struct list_##name **) malloc(sizeof(struct list_##name *))

TEST(free) {
    struct list_int **list = INIT_LIST(int);
    *list = (struct list_int *) malloc(sizeof(struct list_int));

    struct list_int *head = *list;
    struct list_int *second = (struct list_int *) malloc(sizeof(struct list_int));
    struct list_int *third = (struct list_int *) malloc(sizeof(struct list_int));

    head->next = second;
    head->prev = third;

    second->next = third;
    second->prev = head;

    third->next = head;
    third->prev = second;

    list_int_free(list);
    free(list);
}

TEST(create) {
    struct list_int **list = INIT_LIST(int);
    *list = list_int_create(12);

    assert(*list != NULL);
    assert((*list)->val == 12);
    assert((*list)->next == *list);
    assert((*list)->prev == *list);

    list_int_free(list);
    free(list);
}

TEST(push_back) {
    struct list_int **list = INIT_LIST(int);
    
    list_int_push_back(list, 0);
    
    assert((*list) != NULL);
    
    assert((*list)->prev == *list);
    assert((*list)->next == *list);

    list_int_push_back(list, 1);

    assert((*list)->prev);
    assert((*list)->prev->val == 1);
    
    list_int_push_back(list, 2);
    
    assert((*list)->prev);
    assert((*list)->prev->val == 2);
    
    assert((*list)->next);
    assert((*list)->next->val == 1);

    assert((*list)->next->next);
    assert((*list)->next->next->val == 2);

    assert((*list)->next->next->next == (*list));
    assert((*list)->prev->prev->prev == (*list));
}

TEST(push_front) {
    struct list_int **list = INIT_LIST(int);
    
    list_int_push_front(list, 0);

    assert((*list) != NULL);
    assert((*list)->next == *list);
    assert((*list)->prev == *list);

    list_int_push_front(list, -1);

    assert((*list) != NULL);
    assert((*list)->val == -1);
    
    assert((*list)->next != NULL);
    assert((*list)->next->val == 0);

    assert((*list)->prev != NULL);
    assert((*list)->prev->val == 0);

    assert((*list)->next->next == (*list));
    assert((*list)->prev->prev == (*list));

    list_int_push_front(list, -2);

    assert((*list) != NULL);
    assert((*list)->val == -2);

    assert((*list)->next != NULL);
    assert((*list)->next->val == -1);
    assert((*list)->next->next != NULL);
    assert((*list)->next->next->val == 0);
    assert((*list)->next->next->next == (*list));

    assert((*list)->prev != NULL);
    assert((*list)->prev->val == 0);
    assert((*list)->prev->prev != NULL);
    assert((*list)->prev->prev->val == -1);
    assert((*list)->prev->prev->prev == (*list));
}

TEST(front) {
    struct list_int *list = list_int_create(0);
    assert(list_int_front(&list) == 0);

    list_int_push_front(&list, 1);
    assert(list_int_front(&list) == 1);

    list_int_push_back(&list, 2);
    assert(list_int_front(&list) == 1);
}

TEST(back) {
    struct list_int *list = list_int_create(0);
    assert(list_int_back(&list) == 0);

    list_int_push_front(&list, 1);
    assert(list_int_back(&list) == 0);

    list_int_push_back(&list, 2);
    assert(list_int_back(&list) == 2);
}

TEST(pop_front) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 2);
    assert(list_int_pop_front(&list) == 0);
    assert(list_int_pop_front(&list) == 1);
    assert(list_int_pop_front(&list) == 2);
}

TEST(pop_back) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 2);
    assert(list_int_pop_back(&list) == 2);
    assert(list_int_pop_back(&list) == 1);
    assert(list_int_pop_back(&list) == 0);
}

TEST(empty) {
    struct list_int *list = list_int_create(0);
    assert(!list_int_empty(&list));

    list_int_pop_back(&list);
    assert(list_int_empty(&list));

    list_int_push_back(&list, 1);
    assert(!list_int_empty(&list));

    list_int_pop_back(&list);
    assert(list_int_empty(&list));
}

TEST(size) {
    struct list_int *list = list_int_create(0);
    assert(list_int_size(&list) == 1);
    
    list_int_pop_back(&list);
    assert(list_int_size(&list) == 0);
    
    list_int_push_back(&list, 1);
    assert(list_int_size(&list) == 1);
    
    list_int_push_back(&list, 1);
    assert(list_int_size(&list) == 2);

    list_int_pop_back(&list);
    assert(list_int_size(&list) == 1);
}

int main() {
    RUN_TEST(free);
    RUN_TEST(create);
    RUN_TEST(push_back);
    RUN_TEST(push_front);
    RUN_TEST(front);
    RUN_TEST(back);
    RUN_TEST(pop_front);
    RUN_TEST(pop_back);
    RUN_TEST(empty);
    RUN_TEST(size);
    return 0;
}