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
#include <string.h>
#include <errno.h>
#include "cc1.h"
#include "output.h"
#include "error.h"

FILE *output_fp;
struct string *output_path;

/* printf()-style function with our custom format string.

   Specifiers:

   e: print human-readable messaged associated with 'errno' (strerror)

   Specifiers 's', 'd', 'c' are just like printf(). */

void voutput(FILE *fp, char *fmt, va_list args)
{
    while (*fmt) {
        if (*fmt == '%') {
            ++fmt;

            switch (*fmt) {
            case 'c':
                fprintf(fp, "%c", va_arg(args, int));
                break;
            case 'd':
                fprintf(fp, "%d", va_arg(args, int));
                break;
            case 'e':
                fprintf(fp, "%s", strerror(errno));
                break;
            case 's':
                fprintf(fp, "%s", va_arg(args, char *));
                break;
            }
        } else
            putc(*fmt, fp);

        ++fmt;
    }
}

/* write to the output file, using voutput()-style format string */

void output(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    voutput(output_fp, fmt, args);
    va_end(args);
}

/* select the current output section */

void section(enum section new_section)
{
    static enum section old_section = SECTION_NONE;
    char *name;

    if (new_section != old_section) {
        switch (new_section)
        {
        case SECTION_TEXT:      name = "text"; break;
        case SECTION_RODATA:    name = "rodata"; break;
        case SECTION_DATA:      name = "data"; break;

        default: INTERNAL_ERROR("illegal section specified");
        }

        output(".section .%s\n", name);
        old_section = new_section;
    }
}

/* vim: set ts=4 expandtab: */
