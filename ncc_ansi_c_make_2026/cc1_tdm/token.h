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

#ifndef CC1_TOKEN_H
#define CC1_TOKEN_H

#include "string.h"

/* token classes aren't simply opaque arbitrary values. some bits carry
   information useful to various parts of the compiler.

   [8]      TC_FLAG_EXT:    token is not part of standard C90 
   [7:0]    TC_INDEX:       unique index (useful for maps like in output.c) */

#define TC_FLAG_EXT     0x100           
#define TC_INDEX(tc)    ((tc) & 0xFF)

enum token_class
{
    TC_NONE = 0,        /* none/EOF */
    TC_NL = 1,          /* newline */
    TC_HASH = 2,        /* # */
    TC_IDENT = 3,       /* identifier */
    TC_STRLIT = 4,      /* string literal */
    TC_ICON = 5,        /* int constant */
    TC_UCON = 6,        /* unsigned constant */
    TC_LCON = 7,        /* long constant */
    TC_ULCON = 8,       /* unsigned long constant */
    TC_FCON = 9,        /* float constant */
    TC_DCON = 10,       /* double constant */
    TC_LDCON = 11,      /* long double constant */

    /* keywords */

    TC_ASM = (12 | TC_FLAG_EXT),
    TC_AUTO = 13,
    TC_BREAK = 14,
    TC_CASE = 15,
    TC_CHAR = 16,
    TC_CONST = 17,
    TC_CONTINUE = 18,
    TC_DEFAULT = 19,
    TC_DO = 20,
    TC_DOUBLE = 21,
    TC_ELSE = 22,
    TC_ENUM = 23,
    TC_EXTERN = 24,
    TC_FLOAT = 25,
    TC_FOR = 26,
    TC_GOTO = 27,
    TC_IF = 28,
    TC_INLINE = (29 | TC_FLAG_EXT),
    TC_INT = 30,
    TC_LONG = 31,
    TC_REGISTER = 32,
    TC_RETURN = 33,
    TC_SHORT = 34,
    TC_SIGNED = 35,
    TC_SIZEOF = 36,
    TC_STATIC = 37,
    TC_STRUCT = 38,
    TC_SWITCH = 39,
    TC_TYPEDEF = 40,
    TC_UNION = 41,
    TC_UNSIGNED = 42,
    TC_VOID = 43,
    TC_VOLATILE = 44,
    TC_WHILE = 45,

    /* punctuators/operators/separators */

    TC_LPAREN = 46,  /* ( */
    TC_RPAREN = 47,  /* ) */
    TC_LBRACK = 48,  /* [ */
    TC_RBRACK = 49,  /* ] */
    TC_LBRACE = 50,  /* { */
    TC_RBRACE = 51,  /* } */
    TC_QUEST = 52,   /* ? */
    TC_COLON = 53,   /* : */
    TC_SEMI = 54,    /* ; */
    TC_COMMA = 55,   /* , */
    TC_TILDE = 56,   /* ~ */
    TC_DOT = 57,     /* . */
    TC_ELLIP = 58,   /* ... */
    TC_EQ = 59,      /* = */
    TC_EQEQ = 60,    /* == */
    TC_BANG = 61,    /* ! */
    TC_BANGEQ = 62,  /* != */
    TC_MUL = 63,     /* * */
    TC_MULEQ = 64,   /* *= */
    TC_DIV = 65,     /* / */
    TC_DIVEQ = 66,   /* /= */
    TC_MOD = 67,     /* % */
    TC_MODEQ = 68,   /* %= */
    TC_XOR = 69,     /* ^ */
    TC_XOREQ = 70,   /* ^= */
    TC_MINUS = 71,   /* - */
    TC_MINUSEQ = 72, /* -= */
    TC_DEC = 73,     /* -- */
    TC_ARROW = 74,   /* -> */
    TC_PLUS = 75,    /* + */
    TC_PLUSEQ = 76,  /* += */
    TC_INC = 77,     /* ++ */
    TC_AND = 78,     /* & */
    TC_ANDEQ = 79,   /* &= */
    TC_ANDAND = 80,  /* && */
    TC_OR = 81,      /* | */
    TC_OREQ = 82,    /* |= */
    TC_OROR = 83,    /* || */
    TC_GT = 84,      /* > */
    TC_GTEQ = 85,    /* >= */
    TC_SHR = 86,     /* >> */
    TC_SHREQ = 87,   /* >>= */
    TC_LT = 88,      /* < */
    TC_LTEQ = 89,    /* <= */
    TC_SHL = 90,     /* << */
    TC_SHLEQ = 91    /* <<= */
};

struct token
{
    enum token_class class;

    union
    {
        struct string *text;    /* TC_STRLIT or TC_IDENT */
        long l;                 /* TC_ICON or TC_LCON */
        unsigned long ul;       /* TC_UCON or TC_ULCON */
        float f;                /* TC_FCON */
        double d;               /* TC_DCON or TC_LDCON */
    } u;
};

#endif /* CC1_TOKEN_H */

/* vim: set ts=4 expandtab: */
