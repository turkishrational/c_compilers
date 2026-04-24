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

#ifndef CC1_ERROR_H
#define CC1_ERROR_H

#include "string.h"

extern struct string *error_path;
extern int error_line;

void error(char *, ...);

/* these helper macros can't be inline functions because we
   want to report the correct __FILE__ and __LINE__. */

#define INTERNAL_ERROR(msg) \
    error("internal error: '%s' at %s (%d)", msg, __FILE__, __LINE__)

#define ASSERT(cond) do { if (!(cond)) INTERNAL_ERROR(#cond); } while (0)

/* a warning will only be issued if its level is >= 'warning_level'.
   by default, warnings are off. 'warning_level' is controlled by -w.

   for now, only two levels of warnings exist: 

   WL_PROBABLY warnings are for constructs that are probably not intentional,
   and are trivial to rectify: for example, implicit function declarations.
   WL_MAYBE warnings are for constructs that are questionable but might
   nonetheless be used purposely by the programmer. */

enum warning_level
{
    WL_NONE,
    WL_PROBABLY,
    WL_MAYBE
};

extern enum warning_level warning_level;

void warning(int, char *, ...);

#endif /* CC1_ERROR_H */

/* vim: set ts=4 expandtab: */
