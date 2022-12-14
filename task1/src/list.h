#pragma once

#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_

#include <stdlib.h>
#include <stdbool.h>

#define DECLARE_LIST(name, type)                \
struct list_##name {                            \
    type val;                                   \
    struct list_##name *next;                   \
    struct list_##name *prev;                   \
};

#define DEFINE_LIST(name, type)                                                     \
__attribute__((unused))                                                             \
static struct list_##name *list_##name##_create(type value) {                       \
    struct list_##name *res =                                                       \
         (struct list_##name *) malloc(sizeof(struct list_##name));                 \
    res->val = value;                                                               \
    res->next = res;                                                                \
    res->prev = res;                                                                \
    return res;                                                                     \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static void list_##name##_push_back(struct list_##name **list, type value) {        \
    if(!list) return;                                                               \
    struct list_##name *el = list_##name##_create(value);                           \
                                                                                    \
    struct list_##name *head = *list;                                               \
                                                                                    \
    if(!head) { el->prev = el; el->next = el; *list = el; return; }                 \
                                                                                    \
    struct list_##name *last = head->prev;                                          \
                                                                                    \
    last->next = el;                                                                \
    head->prev = el;                                                                \
                                                                                    \
    el->next = head;                                                                \
    el->prev = last;                                                                \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static void list_##name##_push_front(struct list_##name **list, type value){        \
    if(!list) return;                                                               \
    list_##name##_push_back(list, value);                                           \
    *list = (*list)->prev;                                                          \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static type list_##name##_pop_back(struct list_##name **list){                      \
    struct list_##name *head = *list;                                               \
    struct list_##name *last = head->prev;                                          \
                                                                                    \
    if(head == last) {                                                              \
        *list = NULL;                                                               \
        type res = head->val;                                                       \
        free(head);                                                                 \
        return res;                                                                 \
    }                                                                               \
                                                                                    \
    head->prev = last->prev;                                                        \
    last->prev->next = head;                                                        \
                                                                                    \
    type res = last->val;                                                           \
    free(last);                                                                     \
    return res;                                                                     \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static type list_##name##_pop_front(struct list_##name **list) {                    \
    *list = (*list)->next;                                                          \
    return list_##name##_pop_back(list);                                            \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static type list_##name##_front(struct list_##name **list) {                        \
    return (*list)->val;                                                            \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static type list_##name##_back(struct list_##name **list) {                         \
    return (*list)->prev->val;                                                      \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static bool list_##name##_empty(struct list_##name **list) {                        \
    return (list) && !(*list);                                                      \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static size_t list_##name##_size(struct list_##name **list) {                       \
    if(!list) return 0;                                                             \
                                                                                    \
    struct list_##name *head = *list;                                               \
                                                                                    \
    struct list_##name *cur = head;                                                 \
                                                                                    \
    if(!cur) return 0;                                                              \
                                                                                    \
    size_t sz = 1;                                                                  \
    while(cur && cur->next != head) { cur = cur->next; sz++; }                      \
                                                                                    \
    return sz;                                                                      \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static void list_##name##_free(struct list_##name **list) {                         \
    if(!list) return;                                                               \
                                                                                    \
    struct list_##name *head = *list;                                               \
                                                                                    \
    struct list_##name *cur = head;                                                 \
    while(cur && cur->next != head) {                                               \
        struct list_##name *tmp = cur;                                              \
        cur = cur->next;                                                            \
        free(tmp);                                                                  \
    }                                                                               \
}                                                                                   \
                                                                                    \
__attribute__((unused))                                                             \
static void list_##name##_map(struct list_##name **list, void fun(type *)){         \
    if(!list) return;                                                               \
                                                                                    \
    struct list_##name *head = *list;                                               \
    struct list_##name *cur = head;                                                 \
    while(cur && cur->next != head) {                                               \
        fun(&cur->val);                                                             \
        cur = cur->next;                                                            \
    }                                                                               \
    fun(&cur->val);                                                                 \
}

#define DEFINE_LIST_DEBUG(name, type)                                                            \
typedef void (name##_element_printer) (type *);                                                  \
static void list_##name##_print(struct list_##name **list, name##_element_printer p){            \
    list_##name##_map(list, p);                                                                  \
}

#endif /* _DOUBLE_LINKED_LIST_H_ */
