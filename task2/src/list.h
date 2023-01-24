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

#define DEFINE_READ_LIST(name, type)                                                    \
__attribute__((unused))                                                                 \
static inline type list_##name##_front(struct list_##name **list) {                     \
    return (*list)->val;                                                                \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static inline type list_##name##_back(struct list_##name **list) {                      \
    return (*list)->prev->val;                                                          \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static inline bool list_##name##_empty(struct list_##name **list) {                     \
    return (list) && !(*list);                                                          \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static size_t list_##name##_size(struct list_##name **list) {                           \
    if(!list) return 0;                                                                 \
                                                                                        \
    struct list_##name *head = *list;                                                   \
                                                                                        \
    struct list_##name *cur = head;                                                     \
                                                                                        \
    if(!cur) return 0;                                                                  \
                                                                                        \
    size_t sz = 1;                                                                      \
    while(cur && cur->next != head) { cur = cur->next; sz++; }                          \
                                                                                        \
    return sz;                                                                          \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static inline struct list_##name *list_##name##_head(struct list_##name **list) {       \
    return *list;                                                                       \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static inline struct list_##name *list_##name##_last(struct list_##name **list) {       \
    return (*list)->prev;                                                               \
}



#define DEFINE_LIST(name, type)                                                         \
DEFINE_READ_LIST(name, type)                                                            \
__attribute__((unused))                                                                 \
static struct list_##name *list_##name##_create(type value) {                           \
    struct list_##name *res =                                                           \
         (struct list_##name *) malloc(sizeof(struct list_##name));                     \
    res->val = value;                                                                   \
    res->next = res;                                                                    \
    res->prev = res;                                                                    \
    return res;                                                                         \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static void list_##name##_push_back(struct list_##name **list, type value) {            \
    if(!list) return;                                                                   \
    struct list_##name *el = list_##name##_create(value);                               \
                                                                                        \
    struct list_##name *head = *list;                                                   \
                                                                                        \
    if(!head) { el->prev = el; el->next = el; *list = el; return; }                     \
                                                                                        \
    struct list_##name *last = head->prev;                                              \
                                                                                        \
    last->next = el;                                                                    \
    head->prev = el;                                                                    \
                                                                                        \
    el->next = head;                                                                    \
    el->prev = last;                                                                    \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static void list_##name##_push_front(struct list_##name **list, type value){            \
    if(!list) return;                                                                   \
    list_##name##_push_back(list, value);                                               \
    *list = (*list)->prev;                                                              \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static type list_##name##_pop_back(struct list_##name **list){                          \
    struct list_##name *head = *list;                                                   \
    struct list_##name *last = head->prev;                                              \
                                                                                        \
    if(head == last) {                                                                  \
        *list = NULL;                                                                   \
        type res = head->val;                                                           \
        free(head);                                                                     \
        return res;                                                                     \
    }                                                                                   \
                                                                                        \
    head->prev = last->prev;                                                            \
    last->prev->next = head;                                                            \
                                                                                        \
    type res = last->val;                                                               \
    free(last);                                                                         \
    return res;                                                                         \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static type list_##name##_pop_front(struct list_##name **list) {                        \
    *list = (*list)->next;                                                              \
    return list_##name##_pop_back(list);                                                \
}                                                                                       \
                                                                                        \
                                                                                        \
__attribute__((unused))                                                                 \
static void list_##name##_free(struct list_##name **list) {                             \
    if(!list) return;                                                                   \
                                                                                        \
    struct list_##name *head = *list;                                                   \
                                                                                        \
    struct list_##name *cur = head;                                                     \
    while(cur && cur->next != head) {                                                   \
        struct list_##name *tmp = cur;                                                  \
        cur = cur->next;                                                                \
        free(tmp);                                                                      \
    }                                                                                   \
    *list = NULL;                                                                       \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static void list_##name##_map(struct list_##name **list, void fun(type *)){             \
    if(!list) return;                                                                   \
                                                                                        \
    struct list_##name *head = *list;                                                   \
    struct list_##name *cur = head;                                                     \
    while(cur && cur->next != head) {                                                   \
        fun(&(cur->val));                                                               \
        cur = cur->next;                                                                \
    }                                                                                   \
    fun(&(cur->val));                                                                   \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static void list_##name##_insert_after(struct list_##name *el, type value) {            \
    if(!el) return;                                                                     \
                                                                                        \
    struct list_##name *tmp = el->next;                                                 \
    struct list_##name *new_el = list_##name##_create(value);                           \
                                                                                        \
    el->next = new_el;                                                                  \
    new_el->prev = el;                                                                  \
                                                                                        \
    new_el->next = tmp;                                                                 \
    tmp->prev = new_el;                                                                 \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static void list_##name##_remove(struct list_##name **list, struct list_##name *el) {   \
    if(!el) return;                                                                     \
    if(*list == el && el->next == el) { *list = NULL; free(el); return; }               \
                                                                                        \
    if(*list == el) *list = (*list)->next;                                              \
                                                                                        \
    struct list_##name *prev = el->prev;                                                \
    struct list_##name *next = el->next;                                                \
                                                                                        \
    prev->next = next;                                                                  \
    next->prev = prev;                                                                  \
                                                                                        \
    free(el);                                                                           \
}                                                                                       \
                                                                                        \
__attribute__((unused))                                                                 \
static void list_##name##_deep_free(struct list_##name **list,                          \
                                         void free_element(type *)){                    \
    list_##name##_map(list, free_element);                                              \
    list_##name##_free(list);                                                           \
    *list = NULL;                                                                       \
}                                                                                       


#define DEFINE_LIST_DEBUG(name, type)                                                            \
typedef void (name##_element_printer) (type *);                                                  \
static void list_##name##_print(struct list_##name **list, name##_element_printer p){            \
    list_##name##_map(list, p);                                                                  \
}

#endif /* _DOUBLE_LINKED_LIST_H_ */
