#include "test_utils.h"
#include "list.h"

#include <assert.h>
#include <stdlib.h>

// #define DISABLED

DECLARE_LIST(int, int)
DEFINE_LIST(int, int)

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
    *list = NULL;
    
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

    free(list);
}

TEST(push_front) {
    struct list_int **list = INIT_LIST(int);
    *list = NULL;
    
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

    free(list);
}

TEST(front) {
    struct list_int *list = list_int_create(0);
    assert(list_int_front(&list) == 0);

    list_int_push_front(&list, 1);
    assert(list_int_front(&list) == 1);

    list_int_push_back(&list, 2);
    assert(list_int_front(&list) == 1);

    list_int_free(&list);
}

TEST(back) {
    struct list_int *list = list_int_create(0);
    assert(list_int_back(&list) == 0);

    list_int_push_front(&list, 1);
    assert(list_int_back(&list) == 0);

    list_int_push_back(&list, 2);
    assert(list_int_back(&list) == 2);

    list_int_free(&list);
}

TEST(pop_front) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 2);
    assert(list_int_pop_front(&list) == 0);
    assert(list_int_pop_front(&list) == 1);
    assert(list_int_pop_front(&list) == 2);

    list_int_free(&list);
}

TEST(pop_back) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 2);
    assert(list_int_pop_back(&list) == 2);
    assert(list_int_pop_back(&list) == 1);
    assert(list_int_pop_back(&list) == 0);

    list_int_free(&list);
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

    list_int_free(&list);
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
    
    list_int_free(&list);
}

void square(int *el) {
    *el = *el * *el;
}

TEST(map) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 2);
    list_int_push_back(&list, 3);
    list_int_push_back(&list, 4);

    list_int_map(&list, &square);

    assert(list_int_pop_front(&list) == 0);
    assert(list_int_pop_front(&list) == 1);
    assert(list_int_pop_front(&list) == 4);
    assert(list_int_pop_front(&list) == 9);
    assert(list_int_pop_front(&list) == 16);

    list_int_free(&list);
}

TEST(insert_after) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 3);

    list_int_insert_after(list->next, 2);

    assert(list_int_pop_front(&list) == 0);
    assert(list_int_pop_front(&list) == 1);
    assert(list_int_pop_front(&list) == 2);
    assert(list_int_pop_front(&list) == 3);
    assert(list_int_empty(&list));

    list_int_push_back(&list, 1);
    list_int_insert_after(list, 2);

    assert(list->next->next == list);
    assert(list->prev->prev == list);
    
    assert(list_int_pop_front(&list) == 1);
    assert(list_int_pop_front(&list) == 2);

    list_int_free(&list);
}

TEST(remove) {
    struct list_int **list = INIT_LIST(int);
    *list = NULL; 

    list_int_push_back(list, 1);
    list_int_push_back(list, 2);
    list_int_push_back(list, 3);
    list_int_push_back(list, 4);

    list_int_remove(list, (*list)->next);
    assert((*list)->next->val == 3);
    assert((*list)->next->prev == (*list));
    
    list_int_remove(list, (*list)->prev);
    assert((*list)->prev->val == 3);
    assert((*list)->prev->next == (*list));
    assert((*list)->next->val == 3);
    assert((*list)->next->prev == (*list));

    list_int_remove(list, *list);
    assert((*list)->val == 3);
    assert((*list)->next == *list);
    assert((*list)->prev == *list);

    list_int_remove(list, *list);
    assert(list_int_empty(list));

    free(list);
}

void free_int(int *a){}

DECLARE_LIST(string, char *)
DEFINE_LIST(string, char *)

TEST(deep_free_simple) {
    struct list_int **list = INIT_LIST(int);
    *list = NULL;

    list_int_push_back(list, 1);
    list_int_push_back(list, 2);

    list_int_deep_free(list, free_int);

    assert(list_int_empty(list));
    free(list);
}

void free_string(char **str) {
    if(!(*str)) return;
    free(*str);
    *str = NULL;
} 

TEST(deep_free_pointer) {
    struct list_string **list = INIT_LIST(string);
    *list = NULL;

    char *first = (char *) malloc(sizeof(char) * 6);
    strcpy(first, "first");

    char *second = (char *) malloc(sizeof(char) * 7);
    strcpy(second, "second");

    list_string_push_back(list, first);
    list_string_push_back(list, second);

    assert(strcmp(list_string_back(list), "second") == 0);

    list_string_deep_free(list, free_string);
    assert(list_string_empty(list));

    free(list);
}

TEST(head) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_front(&list, -1);

    assert(list_int_head(&list)->val == -1);
    assert(list_int_last(&list)->val == 1);
    assert(list_int_head(&list)->next->val == 0);

    list_int_pop_front(&list);
    list_int_pop_back(&list);

    assert(list_int_head(&list)->val == 0);
    assert(list_int_last(&list)->val == 0);
    assert(list_int_head(&list) == list_int_last(&list));

    list_int_free(&list);
}

bool eq_int(int lhs, int rhs) { return (lhs == rhs); }

TEST(indexof) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 2);
    list_int_push_back(&list, 3);
    list_int_push_back(&list, 4);
    list_int_push_back(&list, 5);
    
    for(size_t i = 0; i < 6; i++) {
        assert(list_int_indexof(&list, i, eq_int) == i);
    }

    list_int_free(&list);
}

TEST(find) {
    struct list_int *list = list_int_create(0);
    list_int_push_back(&list, 1);
    list_int_push_back(&list, 2);
    list_int_push_back(&list, 3);
    list_int_push_back(&list, 4);
    list_int_push_back(&list, 5);
    
    for(size_t i = 0; i < 6; i++) {
        assert(list_int_find(&list, i, eq_int)->val == i);
    }

    list_int_free(&list);
}

int main() {
    #ifndef DISABLED
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
    RUN_TEST(map);
    RUN_TEST(insert_after);
    RUN_TEST(remove);
    RUN_TEST(deep_free_simple);
    RUN_TEST(deep_free_pointer);
    RUN_TEST(indexof);
    RUN_TEST(find);
    #else
    printf("TEST DISABLED\n");
    #endif
    return 0;
}