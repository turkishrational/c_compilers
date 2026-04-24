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

#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "cc1.h"
#include "string.h"
#include "lex.h"
#include "token.h"
#include "error.h"
#include "util.h"

/* the current token: the primary export from the lexical analyzer */

struct token token;

/* pushback buffer for lex_peek(). priming this with a newline is a trick
   to make the logic in lex() work properly on the first call. */

static struct token next = { TC_NL };

/* normally, adjacent string literals are concatenated. setting
   this flag prevents that when processing # line directives. */

static enum bool directive = FALSE;

static char *buffer;            /* lexical buffer */
static int buffer_size = 4096;  /* and its size (set to initial size) */
static char *head;              /* beginning of current token text */
static char *cp;                /* current position */
static char *end;               /* first invalid byte in buffer */

/* discard processed input and refill the buffer from the input file. */

#define LEX_MIN_READ        128

static void refill(void)
{
    int desired;
    int read;

    /* on the first call, there is no buffer */

    if (buffer == NULL) {
        buffer = alloc(buffer_size);
        head = buffer;
        cp = buffer;
        end = buffer;
    }

    /* discard consumed chars and shift the
       rest to the beginning of the buffer */

    if (head != buffer) {
        int shift = head - buffer;

        memmove(buffer, head, end - head);
        head -= shift;
        cp -= shift;
        end -= shift;
    }

    /* grow the buffer if there's still not much room */

    if (((buffer + buffer_size) - end) < LEX_MIN_READ) {
        int new_buffer_size = buffer_size * 2;
        char *new_buffer = alloc(new_buffer_size);

        memcpy(new_buffer, buffer, end - buffer);
        free(buffer);
        head = new_buffer + (head - buffer);
        cp = new_buffer + (cp - buffer);
        end = new_buffer + (end - buffer);
        buffer = new_buffer;
        buffer_size = new_buffer_size;
    }

    /* finally, fill in the free area with as much data from the
       input file as possible. mark EOF with a NUL, which will
       prevent any token-recognition logic from going too far. */

    desired = (buffer + buffer_size) - end;
    read = fread(end, 1, desired, input_fp);
    if (read == -1) error("%e");
    end += read;
    if (read < desired) *end = 0;
}

/* bump 'cp', making sure we don't fall off the end of the buffer */

static inline void advance(void)
{
    if (++cp == end) refill();
}

/* return the value of the decimal/octal/hex digit */

static int digit_value(int c)
{
    if (isdigit(c))
        return c - '0';
    else
        return (toupper(c) - 'A') + 10;
}

/* returns the next character of a char constant or string literal,
   interpreting escape codes. */

static int
escape(void)
{
    int c;

    if (*head == '\\') {
        ++head;
        switch (*head) {
        case 'a': c = '\a'; ++head; break;
        case 'b': c = '\b'; ++head; break;
        case 'f': c = '\f'; ++head; break;
        case 'n': c = '\n'; ++head; break;
        case 'r': c = '\r'; ++head; break;
        case 't': c = '\t'; ++head; break;
        case 'v': c = '\v'; ++head; break;

        case '\\': case '\'': case '\?': case '\"':

            c = *head++;
            break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':

            c = digit_value(*head++);
            if (isdigit(*head) && (*head < '8')) {
                c <<= 3;
                c += digit_value(*head++);
                if (isdigit(*head) && (*head < '8')) {
                    c <<= 3;
                    c += digit_value(*head++);
                }
            }
            if (c > UCHAR_MAX) error("octal escape sequence out of range");
            break;

        case 'x':
            c = 0;
            if (!isxdigit(*++head)) error("malformed hex escape sequence");
            
            while (isxdigit(*head)) {
                if (c & 0xF0) error("hex escape sequence out of range");
                c <<= 4;
                c += digit_value(*head++);
            }
            break;

        default:
            c = *head++;
            warning(WL_PROBABLY, "unknown escape sequence '\\%c'", c);
        }
    } else {
        c = *head++;
    }

    return c;
}

/* consume "delimited" tokens, i.e., character constants and string
   literals. 'delimiter' is ' or ", respectively. upon return, 'cp'
   will point just past the end of this delimited token. */

static void delimit(int delimiter)
{
    int backslash = 1;      /* "1" fakes out first loop */
    int count = -1;         /* don't count opening delimiter */
    
    while (*cp && (*cp != '\n')) {
        if ((*cp == delimiter) && !backslash) break;
        backslash = (*cp == '\\') && !backslash;
        advance();
        ++count;
    }

    if (!*cp || (*cp == '\n')) {
        if (delimiter == '\'') 
            error("unterminated character constant");
        else
            error("unterminated string literal");
    }

    advance();
}



/* this handles common cases with C operators. often an operator '?' may
   appear alone ('?'), doubled ('??'), with an equals sign after it ('?='),
   or doubled with an equals sign after it ('??='). */

static inline int operator(enum token_class alone, enum token_class doubled,
                           enum token_class eq, enum token_class doubled_eq)
{
    if ((doubled_eq != TC_NONE) && (cp[0] == cp[1]) && (cp[2] == '=')) {
        cp += 3;
        return doubled_eq;
    }

    if ((doubled != TC_NONE) && (cp[0] == cp[1])) {
        cp += 2;
        return doubled;
    }

    if ((eq != TC_NONE) && (cp[1] == '=')) {
        cp += 2;
        return eq;
    }

    ++cp;
    return alone;
}

/* innermost scanner: sets 'token.u' if applicable and returns token class */

static int lex0(void)
{
    enum token_class class;
    char *endptr;
    char *tail;
    int newlines;
    int length;

    /*
     * skip all non-newline whitespace and begin a new token.
     */

    for (;;) {
        head = cp;
        if ((end - cp) < 3) refill();
        
        if (isspace(*cp) && (*cp != '\n')) 
            ++cp;
        else
            break;
    }
    
    /*
     * this is a rather large switch, but mostly straightforward. a decent
     * compiler should recognize that this switch covers a small, dense
     * range, and thus implement as a very fast jump table indexed by *cp.
     *
     * note that we can rely on cp[0..2] being valid (or at least terminated
     * with a NUL) because of the logic in the whitespace-skipping code above.
     */

    switch (*cp)
    {
    case 0:     return TC_NONE;
    case '\n':  ++cp; return TC_NL;
    case '#':   ++cp; return TC_HASH;
    case '(':   ++cp; return TC_LPAREN;
    case ')':   ++cp; return TC_RPAREN;
    case '[':   ++cp; return TC_LBRACK;
    case ']':   ++cp; return TC_RBRACK;
    case '{':   ++cp; return TC_LBRACE;
    case '}':   ++cp; return TC_RBRACE;
    case '?':   ++cp; return TC_QUEST;
    case ':':   ++cp; return TC_COLON;
    case ';':   ++cp; return TC_SEMI;
    case ',':   ++cp; return TC_COMMA;
    case '~':   ++cp; return TC_TILDE;

    case '=':   return operator(TC_EQ, TC_NONE, TC_EQEQ, TC_NONE);
    case '!':   return operator(TC_BANG, TC_NONE, TC_BANGEQ, TC_NONE);
    case '&':   return operator(TC_AND, TC_ANDAND, TC_ANDEQ, TC_NONE);
    case '|':   return operator(TC_OR, TC_OROR, TC_OREQ, TC_NONE);
    case '>':   return operator(TC_GT, TC_SHR, TC_GTEQ, TC_SHREQ);
    case '<':   return operator(TC_LT, TC_SHL, TC_LTEQ, TC_SHLEQ);
    case '+':   return operator(TC_PLUS, TC_INC, TC_PLUSEQ, TC_NONE);
    case '*':   return operator(TC_MUL, TC_NONE, TC_MULEQ, TC_NONE);
    case '/':   return operator(TC_DIV, TC_NONE, TC_DIVEQ, TC_NONE);
    case '%':   return operator(TC_MOD, TC_NONE, TC_MODEQ, TC_NONE);
    case '^':   return operator(TC_XOR, TC_NONE, TC_XOREQ, TC_NONE);

    case '.':

        if (isdigit(cp[1])) goto number;

        if ((cp[1] == '.') && (cp[2] == '.')) {
            cp += 3;
            return TC_ELLIP;
        }

        ++cp;
        return TC_DOT;

    case '-':

        if (cp[1] == '>') {
            cp += 2;
            return TC_ARROW;
        }
    
        return operator(TC_MINUS, TC_DEC, TC_MINUSEQ, TC_NONE);

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
    case 'V': case 'W': case 'X': case 'Y': case 'Z': case 'a': case 'b':
    case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
    case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
    case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w':
    case 'x': case 'y': case 'z': case '_':

        while (isalpha(*cp) || isdigit(*cp) || (*cp == '_')) advance();
        token.u.text = string_lookup(head, cp - head);
        class = token.u.text->class;

        if ((class != TC_NONE) && !(strict && (class & TC_FLAG_EXT)))
            return class;
        else
            return TC_IDENT;

    case '0': case '1': case '2': case '3': case '4': case '5': case '6':
    case '7': case '8': case '9':

        number:

        if (*cp == '0') {
            advance();
            if (toupper(*cp) == 'X') {
                advance();
                while (isxdigit(*cp)) advance();
                goto icon;
            }
        }

        while (isdigit(*cp)) advance();

        if ((*cp == '.') || (toupper(*cp) == 'E')) {
            if (*cp == '.') {
                advance();
                while (isdigit(*cp)) advance();
            }

            if (toupper(*cp) == 'E') {
                advance();
                if ((*cp == '-') || (*cp == '+')) advance();
                while (isdigit(*cp)) advance();
            }

            errno = 0;

            if (toupper(*cp) == 'F') {
                advance();
                class = TC_FCON;
                token.u.f = strtof(head, &endptr);
                tail = cp - 1;
            } else {
                if (toupper(*cp) == 'L') {
                    advance();
                    class = TC_LDCON;
                    tail = cp - 1;
                } else {
                    class = TC_DCON;
                    tail = cp;
                }

                token.u.d = strtod(head, &endptr);
            }

            if (errno == ERANGE) error("floating-point constant out of range");
            if (endptr != tail) error("malformed floating-point constant");
            return class;
        }

        icon:

        class = TC_ICON;
        errno = 0;
        tail = cp;
        token.u.ul = strtoul(head, &endptr, 0);
        if (errno == ERANGE) error("integral constant out of range");
        if (endptr != tail) error("malformed integral constant");

        if (toupper(*cp) == 'L') {
            class = TC_LCON;
            advance();
            if (toupper(*cp) == 'U') {
                advance();
                class = TC_ULCON;
            } else {
                if (token.u.ul > LONG_MAX)
                    class = TC_ULCON;
            }
        } else if (toupper(*cp) == 'U') {
            class = TC_UCON;
            advance();
            if (toupper(*cp) == 'L') {
                class = TC_LCON;
                advance();
            } else {
                if (token.u.ul > UINT_MAX)
                    class = TC_ULCON;
            }
        } else { /* unsuffixed */
            if (token.u.ul > INT_MAX) {
                if (*head == '0')
                    class = TC_UCON;
                else
                    class = TC_LCON;
            }

            if (token.u.ul > UINT_MAX) class = TC_LCON;
            if (token.u.ul > LONG_MAX) class = TC_ULCON;
        }

        return class;

    case '"':
        /* we need to concatenate adjacent stirng literals. thus a string
           literal can span lines, so we have to manage lines specially. */

        do {
            delimit('\"');
            newlines = 0;

            while (isspace(*cp) && !directive) {
                if (*cp == '\n') {
                    ++newlines;
                    ++error_line;
                }
                advance();
            }
        } while (*cp == '\"' && !directive);

        /* if we consumed trailing space that included a newline, inject a
           newline back into the stream so 'lex' will handle directives. */

        if (newlines) {
            --cp;
            *cp = '\n'; /* safe because '*cp' must be a whitespace anyway */
            --error_line;
        }

        /* convert the string to its binary representation, in place */

        tail = head;
        length = 0;

        do {
            ++head;

            while (*head != '\"') {
                *tail++ = escape();
                ++length;
            }

            ++head;
            while (isspace(*head)) ++head;
        } while (*head == '\"');

        token.u.text = string_lookup(tail - length, length);
        return TC_STRLIT;

    case '\'':
    
        delimit('\'');
        token.u.l = 0;
        length = 0;
        ++head;

        while (*head != '\'') {
            token.u.l <<= 8;
            token.u.l += escape();
            ++length;
        }

        if (length == 0) error("empty character constant");
        if (length > 1) warning(WL_PROBABLY, "multi-character constant");

        return TC_ICON;

    default:
        error("illegal character (ASCII %d) in input", *cp & 0xFF);
    }
}

/* this middle stage of the pipeline simply reinjects
   tokens buffered by 'lex_peek' into the stream. */

static void lex1(void)
{
    if (next.class != TC_NONE) {
        token = next;
        next.class = TC_NONE;
    } else
        token.class = lex0();
}

/* the outermost lexer 'lex' processes and filters location directives
   from the preprocessor and tracks the current error-reporting location. */

void lex(void)
{
    lex1();

    while (token.class == TC_NL) {
        ++error_line;
        lex1();

        while (token.class == TC_HASH) {
            directive = TRUE;
            lex1();

            if (token.class == TC_ICON) {
                error_line = token.u.l;
                lex1();
                if (token.class == TC_STRLIT) {
                    error_path = token.u.text;
                    lex1();
                }
            }

            /* if we have a malformed directive, we have almost no chance
               of reporting the correct error location here. oh well. */

            if (token.class != TC_NL) error("malformed directive");

            directive = FALSE;
            lex1();
        }
    }
}

/* load the next token into 'lookahead' without consuming it. used by the
   parser in a handful of places where the C grammar isn't regular. */

void lex_peek(struct token *lookahead)
{
    struct token saved;

    saved = token;
    lex();
    *lookahead = token;
    next = token;
    token = saved;
}

/* vim: set ts=4 expandtab: */
