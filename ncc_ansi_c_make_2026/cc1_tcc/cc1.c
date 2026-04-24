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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cc1.h"
#include "string.h"
#include "output.h"
#include "error.h"
#include "lex.h"

extern int optind;

FILE *input_fp;

asm_label_type last_asm_label = ASM_LABEL_NONE; /* asm_label counter */
enum bool strict = TRUE;    /* strict C90 conformance */

/*
 * cc1 [options] <input> <output>
 *
 * options:
 *  -w report warnings (repeat option for more warnings)
 *  -x enable extensions (without this, only C89/C90 is recognized)
 */

int main(int argc, char *argv[])
{
    int opt;

    /*
     * 1. process command-line arguments
     */

    while ((opt = getopt(argc, argv, "wx")) != -1) {
        switch (opt)
        {
        case 'w':
            ++warning_level;
            break;
        case 'x':
            strict = FALSE;
            break;

        default:
            error("malformed option(s)");
        }
    }

    /*
     * 2. open input and output files
     */

    argc -= optind;
    argv += optind;
    if (argc != 2) error("malformed command line");

    output_path = string_lookup(argv[1], strlen(argv[1]));
    error_path = output_path; /* fake out error() temporarily */
    output_fp = fopen(output_path->buf, "w");
    if (output_fp == NULL) error("%e");

    error_path = string_lookup(argv[0], strlen(argv[0]));
    input_fp = fopen(error_path->buf, "r");
    if (input_fp == NULL) error("%e");

    /*
     * 3. compile
     */

    lex();

    while (token.class != TC_NONE) {
        switch (token.class)
        {
        case TC_STRLIT:
            printf("[strlit '%s'] ", token.u.text->buf);
            break;
        case TC_ICON:
            printf("[int %ld/0x%lx] ", token.u.l, token.u.l);
            break;
        case TC_UCON:
            printf("[unsigned %lu/0x%lx] ", token.u.ul, token.u.ul);
            break;
        case TC_LCON:
            printf("[long %ld/0x%lx] ", token.u.l, token.u.l);
            break;
        case TC_ULCON:
            printf("[unsigned long %lu/0x%lx] ", token.u.ul, token.u.ul);
            break;
        case TC_IDENT:
            printf("[ident '%s'] ", token.u.text->buf);
            break;
        default:
            printf("%d ", token.class);
            break;
        }
        lex();
    }

    /* 
     * 4. cleanup
     */

    fclose(input_fp);
    fclose(output_fp);
    exit(0);
}

/* vim: set ts=4 expandtab: */
