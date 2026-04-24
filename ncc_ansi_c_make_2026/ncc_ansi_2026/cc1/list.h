/* Copyright (c) 2019 Charles E. Youse (charles@gnuless.org). 
   All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef CC1_LIST_H
#define CC1_LIST_H

#include <stdlib.h>

#define LIST_DECLARE_TYPES(name, type) \
    struct name##_node { \
        type name; \
        struct name##_node *prev; \
        struct name##_node *next; \
    }; \
    \
    struct name##_list { \
        struct name##_node *head; \
        struct name##_node *tail; \
    };

#define LIST_INTERNAL_INSERT_AFTER(list, node, after) \
    node->prev = after; \
    \
    if (node->prev == NULL) { \
        node->next = list->head; \
        list->head = node; \
    } else { \
        node->next = node->prev->next; \
        node->prev->next = node; \
    } \
    \
    if (node->next == NULL) \
        list->tail = node; \
    else \
        node->next->prev = node;

#define LIST_INTERNAL_INSERT_HEAD(list, node) \
    node->prev = NULL; \
    \
    if (list->head) { \
        node->next = list->head; \
        node->next->prev = node; \
        list->head = node; \
    } else { \
        node->next = NULL; \
        list->head = node; \
        list->tail = node; \
    }

#define LIST_INTERNAL_INSERT_BEFORE(list, node, before) \
    node->next = before; \
    \
    if (node->next == NULL) { \
        node->prev = list->tail; \
        list->tail = node; \
    } else { \
        node->prev = node->next->prev; \
        node->next->prev = node; \
    } \
    \
    if (node->prev == NULL) \
        list->head = node; \
    else \
        node->prev->next = node;

#define LIST_INTERNAL_UNLINK(list, node) \
    if (node->next) \
        node->next->prev = node->prev; \
    else \
        list->tail = node->prev; \
    \
    if (node->prev) \
        node->prev->next = node->next; \
    else \
        list->head = node->next;

#define LIST_INTERNAL_ALLOC(name) \
    struct name##_node *node = alloc(sizeof(struct name##_node)); \
    node->name = name;

#define LIST_DECLARE_INSERT_AFTER(name, type) \
    struct name##_node *name##_list_insert_after(struct name##_list *, \
                                                 type, \
                                                 struct name##_node *);

#define LIST_DEFINE_INSERT_AFTER(name, type) \
    struct name##_node *name##_list_insert_after(struct name##_list *list, \
                                                 type name, \
                                                 struct name##_node *after) \
    { \
        LIST_INTERNAL_ALLOC(name) \
        LIST_INTERNAL_INSERT_AFTER(list, node, after) \
        return node; \
    }

#define LIST_DECLARE_INSERT_HEAD(name, type) \
    struct name##_node *name##_list_insert_head(struct name##_list *, type);

#define LIST_DEFINE_INSERT_HEAD(name, type) \
    struct name##_node *name##_list_insert_head(struct name##_list *list, \
                                                type name) \
    { \
        LIST_INTERNAL_ALLOC(name) \
        LIST_INTERNAL_INSERT_HEAD(list, node) \
        return node; \
    }

#define LIST_DECLARE_INSERT_BEFORE(name, type) \
    struct name##_node *name##_list_insert_before(struct name##_list *, \
                                                  type, \
                                                  struct name##_node *);

#define LIST_DEFINE_INSERT_BEFORE(name, type) \
    struct name##_node *name##_list_insert_before(struct name##_list *list, \
                                                  type name, \
                                                  struct name##_node *after) \
    { \
        LIST_INTERNAL_ALLOC(name) \
        LIST_INTERNAL_INSERT_BEFORE(list, node, after) \
        return node; \
    }

#define LIST_DECLARE_MOVE_AFTER(name, type) \
    void name##_list_move_after(struct name##_list *, \
                                struct name##_node *, \
                                struct name##_node *);

#define LIST_DEFINE_MOVE_AFTER(name, type) \
    void name##_list_move_after(struct name##_list *list, \
                                struct name##_node *node, \
                                struct name##_node *after) \
    { \
        LIST_INTERNAL_UNLINK(list, node) \
        LIST_INTERNAL_INSERT_AFTER(list, node, after) \
    }

#define LIST_DECLARE_MOVE_HEAD(name, type) \
    void name##_list_move_head(struct name##_list *, \
                               struct name##_node *);

#define LIST_DEFINE_MOVE_HEAD(name, type) \
    void name##_list_move_head(struct name##_list *list, \
                               struct name##_node *node) \
    { \
        LIST_INTERNAL_UNLINK(list, node) \
        LIST_INTERNAL_INSERT_HEAD(list, node) \
    }

#define LIST_DECLARE_MOVE_BEFORE(name, type) \
    void name##_list_move_before(struct name##_list *, \
                                 struct name##_node *, \
                                 struct name##_node *);

#define LIST_DEFINE_MOVE_BEFORE(name, type) \
    void name##_list_move_before(struct name##_list *list, \
                                 struct name##_node *node, \
                                 struct name##_node *before) \
    { \
        LIST_INTERNAL_UNLINK(list, node) \
        LIST_INTERNAL_INSERT_BEFORE(list, node, before) \
    }

#define LIST_DECLARE_DELETE(name, type) \
    void name##_list_delete(struct name##_list *list, \
                            struct name##_node *node);

#define LIST_DEFINE_DELETE(name, type) \
    void name##_list_delete(struct name##_list *list, \
                            struct name##_node *node) \
    { \
        LIST_INTERNAL_UNLINK(list, node) \
        free(node); \
    }
    
#endif /* CC1_LIST_H */

/* vim: set ts=4 expandtab: */
