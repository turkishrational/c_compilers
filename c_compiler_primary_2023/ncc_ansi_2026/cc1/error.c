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
#include "cc1.h"
#include "string.h"
#include "output.h"
#include "error.h"

struct string *error_path;
int error_line;

enum warning_level warning_level = WL_NONE;

static void error0(char *kind, char *fmt, va_list args)
{
    if (error_path) {
        fprintf(stderr, "'%s'", error_path->buf);
        if (error_line) fprintf(stderr, " (%d)", error_line);
        fprintf(stderr, ": ");
    } else
        fprintf(stderr, "cc1: ");

    fprintf(stderr, "%s: ", kind);
    voutput(stderr, fmt, args);
    fputc('\n', stderr);
}

/* report an error, clean up, and abort. */

void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    error0("ERROR", fmt, args);
    va_end(args);

    if (output_fp) {
        fclose(output_fp);
        remove(output_path->buf);
    }

    exit(1);
}

/* report a warning, if it meets the threshold */

void warning(int level, char *fmt, ...)
{
    va_list args;

    if (warning_level >= level) {
        va_start(args, fmt);
        error0("WARNING", fmt, args);
        va_end(args);
    }
}

/* vim: set ts=4 expandtab: */
