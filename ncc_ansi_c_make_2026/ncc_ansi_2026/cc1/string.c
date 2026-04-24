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

#include <stdio.h>
#include <string.h>
#include "cc1.h"
#include "string.h"
#include "util.h"

LIST_DEFINE_INSERT_HEAD(string, struct string)
LIST_DEFINE_MOVE_HEAD(string, struct string)

/* all identifiers/keywords and string literals are kept in a hash table for
   the lifetime of the compilation. this simplifies memory management, speeds
   comparisons, and facilitates folding identical string literals. it is a key
   property that, for two 'struct string *' variables 'x' and 'y', 'x == y'
   iff the contents of x.buf and y.buf are identical.

   (this technique is taken from Fraser and Hanson's LCC.)

   the hash function implemented in 'string_hash' is not particularly good,
   excepting that for STRING_NR_BUCKETS==256, the keywords hash perfectly.
   do not change the number of buckets or the hash function unless you are
   prepared to recompute all the prebuilt tables that follow. */

#define STRING_NR_BUCKETS 256

/* return the hash value of a character 'buf' of
   'len' bytes. (no NUL terminator is required.) */

static unsigned string_hash(char *buf, int len)
{
    unsigned hash = 0;
    int i;

    for (i = 0; i < len; ++i, ++buf)
        hash = (hash << 1) ^ hash ^ (*buf & 0xFF);

    return hash;
}

static struct string_node 
    asm_node = { "asm", 3, 0x011d, TC_ASM },
    auto_node = { "auto", 4, 0x037d, TC_AUTO },
    break_node = { "break", 5, 0x05b5, TC_BREAK },
    case_node = { "case", 4, 0x0324, TC_CASE },
    char_node = { "char", 4, 0x0328, TC_CHAR },
    const_node = { "const", 5, 0x0511, TC_CONST },
    continue_node = { "continue", 8, 0x321f, TC_CONTINUE },
    default_node = { "default", 7, 0x11a3, TC_DEFAULT },
    do_node = { "do", 2, 0x00c3, TC_DO },
    double_node = { "double", 6, 0x0f2b, TC_DOUBLE },
    else_node = { "else", 4, 0x033f, TC_ELSE },
    enum_node = { "enum", 4, 0x0337, TC_ENUM },
    extern_node = { "extern", 6, 0x0e42, TC_EXTERN },
    float_node = { "float", 5, 0x0566, TC_FLOAT },
    for_node = { "for", 3, 0x013d, TC_FOR },
    goto_node = { "goto", 4, 0x032d, TC_GOTO },
    if_node = { "if", 2, 0x00dd, TC_IF },
    inline_node = { "inline" ,6, 0x0efb, TC_INLINE },
    int_node = { "int", 3, 0x010b, TC_INT },
    long_node = { "long", 4, 0x0362, TC_LONG },
    register_node = { "register", 8, 0x3fb7, TC_REGISTER },
    return_node = { "return", 6, 0x0c76, TC_RETURN },
    short_node = { "short", 5, 0x042a, TC_SHORT },
    signed_node = { "signed", 6, 0x0c2c, TC_SIGNED },
    sizeof_node = { "sizeof", 6, 0x0cac, TC_SIZEOF },
    static_node = { "static", 6, 0x0da2, TC_STATIC },
    struct_node = { "struct", 6, 0x0d4f, TC_STRUCT },
    switch_node = { "switch", 6, 0x0dfc, TC_SWITCH },
    typedef_node = { "typedef", 7, 0x1781, TC_TYPEDEF },
    union_node = { "union", 5, 0x044d, TC_UNION },
    unsigned_node = { "unsigned", 8, 0x3c49, TC_UNSIGNED },
    void_node = { "void", 4, 0x3fe, TC_VOID },
    volatile_node = { "volatile", 8, 0x3f44, TC_VOLATILE },
    while_node = { "while", 5, 0x0443, TC_WHILE };

static struct string_list buckets[STRING_NR_BUCKETS] = 
{
    [0x0b] = { &int_node, &int_node },
    [0x11] = { &const_node, &const_node },
    [0x1d] = { &asm_node, &asm_node },
    [0x1f] = { &continue_node, &continue_node },
    [0x24] = { &case_node, &case_node },
    [0x28] = { &char_node, &char_node },
    [0x2a] = { &short_node, &short_node },
    [0x2b] = { &double_node, &double_node },
    [0x2c] = { &signed_node, &signed_node },
    [0x2d] = { &goto_node, &goto_node },
    [0x37] = { &enum_node, &enum_node },
    [0x3d] = { &for_node, &for_node },
    [0x3f] = { &else_node, &else_node },
    [0x42] = { &extern_node, &extern_node }, 
    [0x43] = { &while_node, &while_node },
    [0x44] = { &volatile_node, &volatile_node },
    [0x49] = { &unsigned_node, &unsigned_node },
    [0x4d] = { &union_node, &union_node },
    [0x4f] = { &struct_node, &struct_node },
    [0x62] = { &long_node, &long_node },
    [0x66] = { &float_node, &float_node },
    [0x76] = { &return_node, &return_node },
    [0x7d] = { &auto_node, &auto_node },
    [0x81] = { &typedef_node, &typedef_node },
    [0xa2] = { &static_node, &static_node },
    [0xa3] = { &default_node, &default_node },
    [0xac] = { &sizeof_node, &sizeof_node },
    [0xb5] = { &break_node, &break_node },
    [0xb7] = { &register_node, &register_node },
    [0xc3] = { &do_node, &do_node },
    [0xdd] = { &if_node, &if_node },
    [0xfb] = { &inline_node, &inline_node },
    [0xfc] = { &switch_node, &switch_node },
    [0xfe] = { &void_node, &void_node }
};

/* return the 'struct string *' associated with the given string
   'buf' of 'len' bytes, creating a new hash entry if necessary. */

struct string *string_lookup(char *buf, int len)
{
    struct string_list *bucket;
    struct string_node *node;
    struct string string;
    unsigned hash;

    hash = string_hash(buf, len);
    bucket = &buckets[hash % STRING_NR_BUCKETS];

    for (node = bucket->head; node; node = node->next) {
        if (node->string.hash != hash) continue;
        if (node->string.len != len) continue;
        if (memcmp(node->string.buf, buf, len)) continue;
    
        /* looks like this string is already in the table; we shuffle this
           entry to the top of the bucket to keep the list in LRU order. */

        string_list_move_head(bucket, node);
        return &(node->string);
    }

    /* fall through when not found: make a new string entry */

    string.len = len;
    string.hash = hash;
    string.class = TC_NONE;
    string.asm_label = ASM_LABEL_NONE;
    string.buf = alloc(len + 1);
    memcpy(string.buf, buf, len);
    string.buf[len] = 0;
    node = string_list_insert_head(bucket, string);
    return &(node->string);
}

/* vim: set ts=4 expandtab: */
