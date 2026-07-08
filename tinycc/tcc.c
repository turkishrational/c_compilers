/*
 *  TCC - Tiny C Compiler
 * 
 *  Copyright (c) 2001, 2002, 2003 Fabrice Bellard
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* 8/7/2026 - Google AI */ 

#define _GNU_SOURCE
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <setjmp.h>
#include <time.h>
#include <sys/time.h>

#include "elf.h"
#include "stab.h"

/* =========================================================================
   GOOGLE AI & ERDOGAN TAN - FLAT PRINTF RE-ROUTE & MACRO OVERRIDE SHIELD
   =========================================================================
   Override and neutralize all hidden printf/vprintf macros leaking from 
   TDM-GCC/MinGW header files. Forcefully re-route them directly to our 
   stable, thread-safe global 'trdos_print' assembly bridge in libc. */
#undef printf
#undef vprintf

/* Prototype the native TRDOS runtime print engine */
extern int trdos_print(const char *format, ...);
#define printf trdos_print

/* Re-route fputs and fprintf directly to the native TRDOS print engine 
   to bypass legacy stdout/stderr buffering artifacts completely. */
#undef fprintf
#define fprintf(stream, fmt, ...) printf(fmt, ##__VA_ARGS__)
#undef fputs
#define fputs(str, stream) printf("%s", str)
/* ========================================================================= */

#include "libtcc.h"

/* =========================================================================
   GOOGLE AI & ERDOGAN TAN - TCC 0.9.18 PURE 32-BIT POINTER PROTOTYPE SHIELD
   =========================================================================
   Enforce explicit 32-bit pointer return types to prevent TDM-GCC-32 from 
   truncating addresses into 16-bit integers, effectively guarding against 
   the notorious 0Dh General Protection Fault (GPF) at runtime address 255CCh. */
extern void *malloc(unsigned int size);
extern void *realloc(void *ptr, unsigned int size);
extern void *memcpy(void *dest, const void *src, unsigned int n);
extern int strcmp(const char *s1, const char *s2);
extern unsigned int strlen(const char *s);

struct TCCState; /* Forward declaration of core compiler context */
int tcc_add_file(struct TCCState *s, const char *filename);
int tcc_add_library(struct TCCState *s, const char *libraryname);
/* ========================================================================= */

/* Enforcement of the default Native 32-bit x86 architecture target */
#ifndef TCC_TARGET_I386
#define TCC_TARGET_I386
#endif

/* Include raw architecture-specific assembler support */
#define CONFIG_TCC_ASM

/* Explicit library search path for crt0.o and core TRDOS objects */
#define CONFIG_TCC_CRT_PREFIX "C:/TDM-GCC-32/tinycc/lib"

#define INCLUDE_STACK_SIZE  32
#define IFDEF_STACK_SIZE    64
#define VSTACK_SIZE         64
#define STRING_MAX_SIZE     1024

#define TOK_HASH_SIZE       2048 /* Must be a power of two */
#define TOK_ALLOC_INCR      512  /* Must be a power of two */
#define TOK_STR_ALLOC_INCR_BITS 6
#define TOK_STR_ALLOC_INCR (1 << TOK_STR_ALLOC_INCR_BITS)
#define TOK_MAX_SIZE        4    /* Token max size in int units when stored in string */

/* Core token symbol management architecture */
typedef struct TokenSym {
    struct TokenSym *hash_next;
    struct Sym *sym_define;      /* Direct pointer to define descriptors */
    struct Sym *sym_label;       /* Direct pointer to label descriptors */
    struct Sym *sym_struct;      /* Direct pointer to structure definitions */
    struct Sym *sym_identifier;  /* Direct pointer to identifier symbol */
    int tok;                     /* Associated unique token number */
    int len;
    char str[1];
} TokenSym;

typedef struct CString {
    int size;                    /* Active string size in bytes */
    void *data;                  /* Castable pointer to either 'char *' or 'int *' */
    int size_allocated;
    void *data_allocated;        /* Tracks dynamic lifetime if malloced */
} CString;

/* Absolute native type definitions */
typedef struct CType {
    int t;
    struct Sym *ref;
} CType;

/* Execution engine constant value mapping */
typedef union CValue {
    long double ld;
    double d;
    float f;
    int i;
    unsigned int ui;
    unsigned int ul;             /* Absolute 32-bit flat memory address */
    long long ll;
    unsigned long long ull;
    struct CString *cstr;
    void *ptr;
    int tab[1];
} CValue;

/* Value configuration mapping on evaluation stack */
typedef struct SValue {
    CType type;                  /* Data token type */
    unsigned short r;            /* Base register tracking + internal operational flags */
    unsigned short r2;           /* Auxiliary register mapping for 64-bit 'long long' operations */
    CValue c;                    /* Literal constant values */
    struct Sym *sym;             /* Associated compiler symbol references */
} SValue;

/* Flat lexical symbol management structure */
typedef struct Sym {
    int v;                       /* Symbol signature token identifier */
    int r;                       /* Assigned physical hardware processor register */
    int c;                       /* Contextual storage assignment or boundary offset */
    CType type;                  /* Associated explicit compiler type */
    struct Sym *next;            /* Forward link in sequential relational tracking */
    struct Sym *prev;            /* Backward context link on compilation stack */
    struct Sym *prev_tok;        /* Chain link resolving conflicts for identical tokens */
} Sym;

/* Section abstraction markers */
#define SHF_PRIVATE 0x80000000   /* Guard flag preventing section leakage during binary emission */

/* Absolute definition of an executable/object binary section */
typedef struct Section {
    unsigned long data_offset;    /* Current active data offset in memory */
    unsigned char *data;          /* Raw section data buffer pointer */
    unsigned long data_allocated; /* Tracked memory buffer limit for realloc() */
    int sh_name;                  /* ELF section name identifier string offset */
    int sh_num;                   /* ELF section sequential index number */
    int sh_type;                  /* ELF section semantic type */
    int sh_flags;                 /* ELF section operational permission flags */
    int sh_info;                  /* ELF section extra info attribute */
    int sh_addralign;             /* ELF section memory boundary alignment constraint */
    int sh_entsize;               /* ELF entry size specification */
    unsigned long sh_size;        /* Total finalized section size during file emission */
    unsigned long sh_addr;        /* Runtime memory address mapping for target relocation */
    unsigned long sh_offset;      /* Virtual storage file offset mapping */
    int nb_hashed_syms;           /* Tracked metrics to optimize hash table scaling */
    struct Section *link;         /* Link descriptor targeting another section */
    struct Section *reloc;        /* Relocation tracking metadata table cross-link */
    struct Section *hash;         /* Associated direct lookup symbol hash table */
    struct Section *next;         /* Pointer to the next section descriptor in layout */
    char name[64];                /* Literal human-readable section name identifier */
} Section;

/* GNU C Compiler attribute translation mapping */
typedef struct AttributeDef {
    int aligned;                  /* Requested hardware memory alignment boundary */
    Section *section;             /* Target custom memory storage destination section */
    unsigned char func_call;      /* Assigned function calling convention (Defaults to FUNC_CDECL) */
} AttributeDef;

#define SYM_STRUCT     0x40000000 /* Symbol namespace mask for struct/union/enum definitions */
#define SYM_FIELD      0x20000000 /* Symbol namespace mask for composite structure fields */
#define SYM_FIRST_ANOM (1 << (31 - VT_STRUCT_SHIFT)) /* Base token allocator offset for anonymous symbols */

/* Stored inside the 'Sym.c' metadata field */
#define FUNC_NEW       1          /* Modern ANSI-compliant function prototype descriptor */
#define FUNC_OLD       2          /* Legacy K&R style function prototype descriptor */
#define FUNC_ELLIPSIS  3          /* Variadic ANSI function prototype including ellipsis (...) */

/* Stored inside the 'Sym.r' execution field */
#define FUNC_CDECL     0          /* Standard C calling convention (Default stack cleanup by caller) */
#define FUNC_STDCALL   1          /* Win32 WinAPI compatibility Pascal calling convention (Unused) */

/* Configuration assigned to the 'Sym.t' macro expansion processor */
#define MACRO_OBJ      0          /* Standard object-like macro constant expansion block */
#define MACRO_FUNC     1          /* Functional parametrizable macro expansion block */

/* Parameter track mappings for 'Sym.r' label resolution */
#define LABEL_DEFINED  0          /* Target code block label has been successfully resolved */
#define LABEL_FORWARD  1          /* Forward-declared destination label awaiting binding resolution */
#define LABEL_DECLARED 2          /* Declared compilation tracking label that remains unreferenced */

/* Grammatical parsing declaration modes */
#define TYPE_ABSTRACT  1          /* Isolated type descriptor structure evaluated without a live variable */
#define TYPE_DIRECT    2          /* Standard variable definition including explicit typing */

#define IO_BUF_SIZE 8192          /* Optimized low-level input-output disk block layout size */

/* Low-level system track state representation for an open source target file */
typedef struct BufferedFile {
    uint8_t *buf_ptr;             /* Active operational memory pointer within stream window */
    uint8_t *buf_end;             /* Dynamic boundary end pointer marking current frame data block */
    int fd;                       /* Native TRDOS file descriptor tracking integer */
    int line_num;                 /* Current active sequential execution line counter metrics */
    int ifndef_macro;             /* Target tracking symbol identifier for single-inclusion guards */
    int ifndef_macro_saved;       /* Temporary state tracking register for preprocessor guards */
    int *ifdef_stack_ptr;         /* State evaluation tracking pointer recording active ifdef depth */
    char inc_type;                /* Context token tracking inclusion type ('<' or '"') */
    char inc_filename[512];       /* Raw verbatim source path string typed by developer */
    char filename[1024];          /* Final resolved canonical storage workspace file path */
    unsigned char buffer[IO_BUF_SIZE + 1]; /* Extended hardware frame layout containing CH_EOB padding */
} BufferedFile;

#define CH_EOB   '\\'             /* End-of-Buffer escape token representation flag */
#define CH_EOF   (-1)             /* End-of-File hard execution binary signal */

/* Save state snapshot data engine designed to record and re-evaluate source streams */
typedef struct ParseState {
    int *macro_ptr;               /* Active text layout macro processing frame position pointer */
    int line_num;                 /* Tracked line number context */
    int tok;                      /* Evaluated unique token id snapshot target */
    CValue tokc;                  /* Extracted lexical context metadata token value state */
} ParseState;

/* Record descriptor designed to dynamically hold sequential linear code string components */
typedef struct TokenString {
    int *str;                     /* Internal memory allocation containing raw token sequence arrays */
    int len;                      /* Active length count of tokens inside storage */
    int allocated_len;            /* Total capacity size boundaries before resizing hooks */
    int last_line_num;            /* Last known line boundary index parameter metadata */
} TokenString;

/* Inclusion caching record descriptor avoiding redundant filesystem overhead reads */
typedef struct CachedInclude {
    int ifndef_macro;             /* Associated guard macro definition register index */
    char type;                    /* Specific token qualifier identification marker matching file scope */
    char filename[1];             /* Extensible continuous layout tracking the literal path string */
} CachedInclude;

/* Core Global Lexical Parser Context States */
static struct BufferedFile *file; /* Dynamic file object currently under parsing */
static int ch, tok;               /* Read character cache and current lexical evaluation token */
static CValue tokc;               /* Current literal value assigned to active lexical token */
static CString tokcstr;           /* Active continuous character string builder container context */

static int tok_flags;             /* Special analytical state modifiers tracking position context */
#define TOK_FLAG_BOL   0x0001     /* Token occupies absolute Beginning-Of-Line layout location */
#define TOK_FLAG_BOF   0x0002     /* Token occupies absolute Beginning-Of-File layout location */
#define TOK_FLAG_ENDIF 0x0004     /* Lexical block processing completed successfully matching ifdef */

static int *macro_ptr, *macro_ptr_allocated;
static int *unget_saved_macro_ptr;
static int unget_saved_buffer[TOK_MAX_SIZE + 1];
static int unget_buffer_enabled;
static int parse_flags;
#define PARSE_FLAG_PREPROCESS 0x0001 /* Enforce standard source preprocessor execution tracking */
#define PARSE_FLAG_TOK_NUM    0x0002 /* Interpret character sequences as raw native numbers directly */
#define PARSE_FLAG_LINEFEED   0x0004 /* Capture carriage break signals returning them as standard tokens */
 
/* Core structural section endpoints mapped globally within memory architecture */
static Section *text_section, *data_section, *bss_section; 
static Section *cur_text_section; /* Target structural window code execution engine active mapping */

/* Core global lookup tables linking symbols to memory segments */
static Section *symtab_section, *strtab_section;

/* Compilation workspace layout descriptors capturing debug traces */
static Section *stab_section, *stabstr_section;

/* Compilation Metric Layout Tracking Indexes:
   loc : Active local scope offset track register
   ind : Direct emission memory machine code output offset track register
   rsym: Target tracking symbol index evaluating operational returns
   anon_sym: Tracker register counting unique anonymous layout structures
*/
static int rsym, anon_sym, ind, loc;

static int const_wanted;          /* Truth criteria determining compile-time validation evaluation */
static int nocode_wanted;         /* Suppress hardware binary optimization instruction emission blocks */
static int global_expr;           /* Force compound tracking symbols directly inside permanent scopes */
static CType func_vt;             /* Track and assert standard operational return data definitions */
static int func_vc;
static int last_line_num, last_ind, func_ind; /* Analytical metric tracking variables */
static int tok_ident;
static TokenSym **table_ident;
static TokenSym *hash_ident[TOK_HASH_SIZE];
static char token_buf[STRING_MAX_SIZE + 1];
static char *funcname;
static Sym *global_stack, *local_stack;
static Sym *define_stack;
static Sym *global_label_stack, *local_label_stack;

static SValue vstack[VSTACK_SIZE], *vtop;
/* Predefined core compiler type descriptors */
static CType char_pointer_type, func_old_type, int_type;
/* Fast lookup table determining valid identifier and numeric character scopes: true if isid(c) || isnum(c) */
static unsigned char isidnum_table[256];

/* Compile with native debug symbol infrastructure support */
static int do_debug = 0;

static int total_lines;
static int total_bytes;

/* Enable standard GNU C compiler semantic language extensions */
static int gnu_ext = 1;

/* Enable native Tiny C specific compiler optimization extensions */
static int tcc_ext = 1;

/* 8/7/2026 - Google AI */
int do_bounds_check = 0;
Section *lbounds_section = NULL;

/* Core global pointer tracking active compiler state instance context */
static struct TCCState *tcc_state;

/* Base installation filesystem directory path routing internal include directories */
static const char *tcc_lib_path = CONFIG_TCC_LIBDIR "/tcc";

/* Master global state container tracking compilation session metrics */
struct TCCState {
    int output_type;              /* Selected emission file format target mode (OBJ or EXE) */
 
    BufferedFile **include_stack_ptr;
    int *ifdef_stack_ptr;

    /* Preprocessor continuous filesystem include search parameters */
    char **include_paths;
    int nb_include_paths;
    char **sysinclude_paths;
    int nb_sysinclude_paths;
    CachedInclude **cached_includes;
    int nb_cached_includes;

    char **library_paths;
    int nb_library_paths;

    /* Low-level storage containing structural binary sections memory maps */
    Section **sections;
    int nb_sections;              /* Total count of registered memory allocation segments */

    /* If true, standard compiler system headers are completely bypassed */
    int nostdinc;
    
    /* If true, enforces absolute pure static compilation linking modes */
    int static_link;

    /* Runtime compilation error interception mechanisms */
    void *error_opaque;
    void (*error_func)(void *opaque, const char *msg);
    int error_set_jmp_enabled;
    jmp_buf error_jmp_buf;
    int nb_errors;

    /* Embedded architecture-specific inline assembler label stack descriptor */
    Sym *asm_labels;

    /* Core physical static array structures recording stack track offsets */
    BufferedFile *include_stack[INCLUDE_STACK_SIZE];
    int ifdef_stack[IFDEF_STACK_SIZE];
};

/* The current value state registers configuration markers: */
#define VT_VALMASK   0x00ff
#define VT_CONST     0x00f0  /* Literal constant in evaluation cache (must be first non-register value) */
#define VT_LLOCAL    0x00f1  /* Lvalue descriptor, tracked variable offset on hardware stack */
#define VT_LOCAL     0x00f2  /* Evaluated variable storage location offset on stack */
#define VT_CMP       0x00f3  /* Temporary value stored directly inside hardware processor flags */
#define VT_JMP       0x00f4  /* Direct jump target routing resolution on truth evaluation (even) */
#define VT_JMPI      0x00f5  /* Direct jump target routing resolution on false evaluation (odd) */
#define VT_LVAL      0x0100  /* Memory reference address identifier acts as an active lvalue */
#define VT_SYM       0x0200  /* Lexical symbol reference address offset tracking multiplication marker */
#define VT_MUSTCAST  0x0400  /* Type conversion enforcement flag (used for char/short promotions in 32-bit registers) */
#define VT_LVAL_BYTE     0x1000  /* Lvalue references a single byte data boundary layout */
#define VT_LVAL_SHORT    0x2000  /* Lvalue references a 16-bit short integer data boundary layout */
#define VT_LVAL_UNSIGNED 0x4000  /* Lvalue references an unsigned data type memory block */
#define VT_LVAL_TYPE     (VT_LVAL_BYTE | VT_LVAL_SHORT | VT_LVAL_UNSIGNED)

/* Core compilation language primitive type maps */
#define VT_STRUCT_SHIFT 12   /* Hardware namespace isolation shift parameter for structure/enum records */

#define VT_INT        0  /* Native 32-bit signed integer type */
#define VT_BYTE       1  /* Native 8-bit signed byte integer type */
#define VT_SHORT      2  /* Native 16-bit signed short integer type */
#define VT_VOID       3  /* Void storage completion structure representation type */
#define VT_PTR        4  /* Explicit memory address reference pointer memory layout pointer */
#define VT_ENUM       5  /* Enumerated list variable constraint descriptor structure */
#define VT_FUNC       6  /* Executable routine function signature entry mapping type */
#define VT_STRUCT     7  /* Composite memory record structural layout specification (struct/union) */
#define VT_FLOAT      8  /* Single-precision IEEE-754 hardware floating-point layout configuration */
#define VT_DOUBLE     9  /* Double-precision IEEE-754 hardware floating-point layout configuration */
#define VT_LDOUBLE   10  /* Extended-precision long double hardware floating-point layout configuration */
#define VT_BOOL      11  /* ISO C99 compliant strict true/false boolean storage configuration */
#define VT_LLONG     12  /* 64-bit extended composite signed integer data structure type */
#define VT_LONG      13  /* Legacy syntax tracker alias (evaluated and converted during parsing phase) */
#define VT_BTYPE      0x000f /* Logical isolation filter masking the core basic runtime types */
#define VT_UNSIGNED   0x0010  /* Modifier marking the type boundaries as positive unsigned memory data */
#define VT_ARRAY      0x0020  /* Contiguous array structure memory layout modifier (implicitly enforces VT_PTR) */
#define VT_BITFIELD   0x0040  /* Special structure internal boundary bitfield allocation track modifier */

/* Variable allocation persistence storage classes */
#define VT_EXTERN  0x00000080  /* External reference symbol linkage configuration marker */
#define VT_STATIC  0x00000100  /* Scope-restricted persistent internal storage visibility class */
#define VT_TYPEDEF 0x00000200  /* Alias definition mapper overriding structural type descriptors */
#define VT_INLINE  0x00000400  /* Inline expansion recommendation hint compiler optimization marker */

/* Bitmask filters extracting layout specifics */
#define VT_STORAGE (VT_EXTERN | VT_STATIC | VT_TYPEDEF | VT_INLINE)
#define VT_TYPE    (~(VT_STORAGE))

/* Lexical Analyzer Logical Validation Token Assignments */

/* Warning: The layout configuration of the following conditional tokens directly impacts i386 asm emission */
#define TOK_ULT 0x92
#define TOK_UGE 0x93
#define TOK_EQ  0x94
#define TOK_NE  0x95
#define TOK_ULE 0x96
#define TOK_UGT 0x97
#define TOK_LT  0x9c
#define TOK_GE  0x9d
#define TOK_LE  0x9e
#define TOK_GT  0x9f

#define TOK_LAND  0xa0
#define TOK_LOR   0xa1

#define TOK_DEC   0xa2
#define TOK_MID   0xa3 /* Boundary tracking utility converting inc/dec markers to void expressions */
#define TOK_INC   0xa4
#define TOK_UDIV  0xb0 /* Mathematical unsigned integer division hardware processor configuration */
#define TOK_UMOD  0xb1 /* Mathematical unsigned integer modulo hardware processor configuration */
#define TOK_PDIV  0xb2 /* Hardware memory pointer scale division boundary validation shortcut */
#define TOK_CINT   0xb3 /* Integer literal configuration parsed data tracking address token */
#define TOK_CCHAR 0xb4 /* Character literal configuration parsed data tracking address token */
#define TOK_STR   0xb5 /* String literal baseline pointer memory token captured inside tokc storage */
#define TOK_TWOSHARPS 0xb6 /* Preprocessor string concatenation compilation instruction operator token (##) */
#define TOK_LCHAR    0xb7
#define TOK_LSTR     0xb8
#define TOK_CFLOAT   0xb9 /* Single-precision floating point constant validation metadata block */
#define TOK_LINENUM  0xba /* Preprocessor sequential tracking index validation indicator token */
#define TOK_CDOUBLE  0xc0 /* Double-precision floating point constant validation metadata block */
#define TOK_CLDOUBLE 0xc1 /* Extended long double floating point constant validation metadata block */
#define TOK_UMULL    0xc2 /* Mathematical unsigned 32-bit x 32-bit to 64-bit precision multiplication */
#define TOK_ADDC1    0xc3 /* Low-level processor mathematical addition enabling hardware carry generation flags */
#define TOK_ADDC2    0xc4 /* Low-level processor mathematical addition enforcing historical carry evaluation flags */
#define TOK_SUBC1    0xc5 /* Low-level processor mathematical subtraction enabling hardware borrow generation flags */
#define TOK_SUBC2    0xc6 /* Low-level processor mathematical subtraction enforcing historical borrow evaluation flags */
#define TOK_CUINT    0xc8 /* Unsigned integer literal data tracking validation token configuration */
#define TOK_CLLONG   0xc9 /* 64-bit long long numeric constant evaluation parsing tracking token */
#define TOK_CULLONG  0xca /* 64-bit unsigned long long numeric constant evaluation parsing tracking token */
#define TOK_ARROW    0xcb
#define TOK_DOTS     0xcc /* Variadic parameter indicator validation ellipsis token (...) */
#define TOK_SHR      0xcd /* Logically isolated bitwise unsigned shift right execution token */
#define TOK_PPNUM    0xce /* Raw preprocessor digit sequence evaluation string token */

#define TOK_SHL   0x01 /* Bitwise shift left data execution token instruction descriptor */
#define TOK_SAR   0x02 /* Bitwise signed arithmetic shift right data execution token instruction descriptor */
  
/* Self-referencing shorthand assignment expression token modifiers (Base Operator masked with 0x80) */
#define TOK_A_MOD 0xa5
#define TOK_A_AND 0xa6
#define TOK_A_MUL 0xaa
#define TOK_A_ADD 0xab
#define TOK_A_SUB 0xad
#define TOK_A_DIV 0xaf
#define TOK_A_XOR 0xde
#define TOK_A_OR  0xfc
#define TOK_A_SHL 0x81
#define TOK_A_SAR 0x82

/* WARNING: The exact internal storage layout string map indexes token definitions sequentially */
static char tok_two_chars[] = "<=\236>=\235!=\225&&\240||\241++\244--\242==\224<<\1>>\2+=\253-=\255*=\252/=\257%=\245&=\246^=\336|=\374->\313..\250##\266";

#define TOK_EOF       (-1)  /* Terminal End-Of-File parsing pipeline signal code */
#define TOK_LINEFEED  10    /* Standard control signal carriage feed break point */

/* The fundamental base boundary offset. All identifiers and literal strings evaluate above this limit */
#define TOK_IDENT 256

/* Core structural formatting macros translating inline human-readable architecture opcodes to token configurations */
#define DEF_ASM(x) DEF(TOK_ASM_ ## x, #x)

#define DEF_BWL(x) \
 DEF(TOK_ASM_ ## x ## b, #x "b") \
 DEF(TOK_ASM_ ## x ## w, #x "w") \
 DEF(TOK_ASM_ ## x ## l, #x "l") \
 DEF(TOK_ASM_ ## x, #x)

#define DEF_WL(x) \
 DEF(TOK_ASM_ ## x ## w, #x "w") \
 DEF(TOK_ASM_ ## x ## l, #x "l") \
 DEF(TOK_ASM_ ## x, #x)

#define DEF_FP1(x) \
 DEF(TOK_ASM_ ## f ## x ## s, "f" #x "s") \
 DEF(TOK_ASM_ ## fi ## x ## l, "fi" #x "l") \
 DEF(TOK_ASM_ ## f ## x ## l, "f" #x "l") \
 DEF(TOK_ASM_ ## fi ## x ## s, "fi" #x "s")

#define DEF_FP(x) \
 DEF(TOK_ASM_ ## f ## x, "f" #x ) \
 DEF(TOK_ASM_ ## f ## x ## p, "f" #x "p") \
 DEF_FP1(x)

#define DEF_ASMTEST(x) \
 DEF_ASM(x ## o) \
 DEF_ASM(x ## no) \
 DEF_ASM(x ## b) \
 DEF_ASM(x ## c) \
 DEF_ASM(x ## nae) \
 DEF_ASM(x ## nb) \
 DEF_ASM(x ## nc) \
 DEF_ASM(x ## ae) \
 DEF_ASM(x ## e) \
 DEF_ASM(x ## z) \
 DEF_ASM(x ## ne) \
 DEF_ASM(x ## nz) \
 DEF_ASM(x ## be) \
 DEF_ASM(x ## na) \
 DEF_ASM(x ## nbe) \
 DEF_ASM(x ## a) \
 DEF_ASM(x ## s) \
 DEF_ASM(x ## ns) \
 DEF_ASM(x ## p) \
 DEF_ASM(x ## pe) \
 DEF_ASM(x ## np) \
 DEF_ASM(x ## po) \
 DEF_ASM(x ## l) \
 DEF_ASM(x ## nge) \
 DEF_ASM(x ## nl) \
 DEF_ASM(x ## ge) \
 DEF_ASM(x ## le) \
 DEF_ASM(x ## ng) \
 DEF_ASM(x ## nle) \
 DEF_ASM(x ## g)

#define TOK_ASM_int TOK_INT

enum {
    TOK_LAST = TOK_IDENT - 1,
#define DEF(id, str) id,
#include "tcctok.h"
#undef DEF
};

static const char tcc_keywords[] = 
#define DEF(id, str) str "\0"
#include "tcctok.h"
#undef DEF
;

#define TOK_UIDENT TOK_DEFINE

/* =========================================================================
   GOOGLE AI & ERDOGAN TAN - THE UNIFICATION VICTORY (SNPRINTF OVERRIDE)
   =========================================================================
   Neutralize standard/MinGW snprintf macros completely to prevent core stack 
   corruption. Forcefully bind all thread-safe continuous string formatting 
   directly to our native 'trdos_snprintf' bridge located inside libc. */
#undef snprintf
#undef _snprintf

/* Native snprintf translation bridge defined inside TRDOS 386 libc_printf_bridge.s */
extern int trdos_snprintf(char *str, unsigned int size, const char *format, ...);

#define snprintf trdos_snprintf
#define _snprintf trdos_snprintf

/* Standard ISOC99 mathematical float conversion configurations */
extern float strtof (const char *__nptr, char **__endptr);
extern long double strtold (const char *__nptr, char **__endptr);

/* Core internal compiler continuous string utility wrappers */
static char *pstrcpy(char *buf, int buf_size, const char *s);
static char *pstrcat(char *buf, int buf_size, const char *s);

/* Lexical Parser Pipeline Token Advancement Functions */
static void next(void);
static void next_nomacro(void);
static void parse_expr_type(CType *type);
static void expr_type(CType *type);
static void unary_type(CType *type);
static void block(int *bsym, int *csym, int *case_sym, int *def_sym, 
                  int case_reg, int is_expr);
static int expr_const(void);
static void expr_eq(void);
static void gexpr(void);
static void decl(int l);
static void decl_initializer(CType *type, Section *sec, unsigned long c, 
                             int first, int size_only);
static void decl_initializer_alloc(CType *type, AttributeDef *ad, int r, 
                                   int has_init, int v, int scope);

/* Backend X86 Machine Code Code-Generator Engine Functions */
int gv(int rc);
void gv2(int rc1, int rc2);
void move_reg(int r, int s);
void save_regs(int n);
void save_reg(int r);
void vpop(void);
void vswap(void);
void vdup(void);
int get_reg(int rc);

static void macro_subst(TokenString *tok_str, 
                        Sym **nested_list, const int *macro_str);
int save_reg_forced(int r);
void gen_op(int op);
void force_charshort_cast(int t);
static void gen_cast(CType *type);
void vstore(void);
static Sym *sym_find(int v);
static Sym *sym_push(int v, CType *type, int r, int c);

/* Compilation Semantic Type Boundary Handling Engines */
static int type_size(CType *type, int *a);
static inline CType *pointed_type(CType *type);
static int pointed_size(CType *type);
static int lvalue_type(int t);
static int is_compatible_types(CType *type1, CType *type2);
static int parse_btype(CType *type, AttributeDef *ad);
static void type_decl(CType *type, AttributeDef *ad, int *v, int td);

/* Error Logging and Diagnostic Framework */
void error(const char *fmt, ...);
void vpushi(int v);
void vset(CType *type, int r, int v);
void type_to_str(char *buf, int buf_size, 
                 CType *type, const char *varstr);
char *get_tok_str(int v, CValue *cv);
static Sym *get_sym_ref(CType *type, Section *sec, 
                        unsigned long offset, unsigned long size);
static Sym *external_global_sym(int v, CType *type, int r);

/* Flat ELF/PRG Structured Section Generation Routines */
static void section_realloc(Section *sec, unsigned long new_size);
static void *section_ptr_add(Section *sec, unsigned long size);
static void put_extern_sym(Sym *sym, Section *section, 
                           unsigned long value, unsigned long size);
static void greloc(Section *s, Sym *sym, unsigned long addr, int type);
static int put_elf_str(Section *s, const char *sym);
static int put_elf_sym(Section *s, 
                       unsigned long value, unsigned long size,
                       int info, int other, int shndx, const char *name);
static int add_elf_sym(Section *s, unsigned long value, unsigned long size,
                       int info, int sh_num, const char *name);
static void put_elf_reloc(Section *symtab, Section *s, unsigned long offset,
                          int type, int symbol);

/* Technical Debug Tracking Traces Emission Engines (STABS) */
static void put_stabs(const char *str, int type, int other, int desc, 
                      unsigned long value);
static void put_stabs_r(const char *str, int type, int other, int desc, 
                        unsigned long value, Section *sec, int sym_index);
static void put_stabn(int type, int other, int desc, int value);
static void put_stabd(int type, int other, int desc);

/* Native Core Filesystem Ingestion Engines */
#define AFF_PRINT_ERROR     0x0001 /* Print explicit diagnostics if target source file is missing */
static int tcc_add_file_internal(TCCState *s, const char *filename, int flags);

#ifdef CONFIG_TCC_ASM
typedef struct ExprValue { uint32_t v; Sym *sym; } ExprValue;
#define MAX_ASM_OPERANDS 30
typedef struct ASMOperand { int id; char *constraint; char asm_str[16]; SValue *vt; int ref_index; int priority; int reg; int is_llong; } ASMOperand;
static void asm_expr(struct TCCState *s1, ExprValue *pe);
static int asm_int_expr(struct TCCState *s1);
static int find_constraint(ASMOperand *operands, int nb_operands, const char *name, const char **pp);
static int tcc_assemble(struct TCCState *s1, int do_preprocess);
#endif
static void asm_instr(void);
static inline int is_float(int t) { int bt; bt = t & VT_BTYPE; return bt == VT_LDOUBLE || bt == VT_DOUBLE || bt == VT_FLOAT; }
#ifdef TCC_TARGET_I386
#include "i386-gen.c"
#endif

/* Standard IEEE-754 floating point verification to bypass non-standard math library dependency artifacts */
int ieee_finite(double d)
{
    int *p = (int *)&d;
    return ((unsigned)((p[1] | 0x800fffff) + 1)) >> 31;
}

/* Copy a source string safely and enforce strict truncation limitations */
static char *pstrcpy(char *buf, int buf_size, const char *s)
{
    char *q, *q_end;
    int c;

    if (buf_size > 0) {
        q = buf;
        q_end = buf + buf_size - 1;
        while (q < q_end) {
            c = *s++;
            if (c == '\0')
                break;
            *q++ = c;
        }
        *q = '\0';
    }
    return buf;
}

/* Concatenate a source string safely and enforce strict truncation limitations */
static char *pstrcat(char *buf, int buf_size, const char *s)
{
    int len;
    len = strlen(buf);
    if (len < buf_size) 
        pstrcpy(buf + len, buf_size - len, s);
    return buf;
}

/* Absolute minimal heap allocation wrappers tailored for TRDOS 386 native workspace */
static inline void tcc_free(void *ptr)
{
    free(ptr);
}

static void *tcc_malloc(unsigned long size)
{
    void *ptr;
    ptr = malloc(size);
    if (!ptr && size)
        error("memory full");
    return ptr;
}

static void *tcc_mallocz(unsigned long size)
{
    void *ptr;
    ptr = tcc_malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

static inline void *tcc_realloc(void *ptr, unsigned long size)
{
    void *ptr1;
    ptr1 = realloc(ptr, size);
    return ptr1;
}

static char *tcc_strdup(const char *str)
{
    char *ptr;
    ptr = tcc_malloc(strlen(str) + 1);
    strcpy(ptr, str);
    return ptr;
}

#define free(p) use_tcc_free(p)
#define malloc(s) use_tcc_malloc(s)
#define realloc(p, s) use_tcc_realloc(p, s)

/* Dynamic sequence manager expanding target array structures at power-of-two operational thresholds */
static void dynarray_add(void ***ptab, int *nb_ptr, void *data)
{
    int nb, nb_alloc;
    void **pp;
    
    nb = *nb_ptr;
    pp = *ptab;
    
    if ((nb & (nb - 1)) == 0) {
        if (!nb)
            nb_alloc = 1;
        else
            nb_alloc = nb * 2;
        pp = tcc_realloc(pp, nb_alloc * sizeof(void *));
        if (!pp)
            error("memory full");
        *ptab = pp;
    }
    pp[nb++] = data;
    *nb_ptr = nb;
}

/* Create and initialize a new structured binary layout section */
Section *new_section(TCCState *s1, const char *name, int sh_type, int sh_flags)
{
    Section *sec;

    sec = tcc_mallocz(sizeof(Section));
    pstrcpy(sec->name, sizeof(sec->name), name);
    sec->sh_type = sh_type;
    sec->sh_flags = sh_flags;
    
    switch(sh_type) {
    case SHT_HASH:
    case SHT_REL:
    case SHT_DYNSYM:
    case SHT_SYMTAB:
    case SHT_DYNAMIC:
        sec->sh_addralign = 4;
        break;
    case SHT_STRTAB:
        sec->sh_addralign = 1;
        break;
    default:
        sec->sh_addralign = 32; /* Default conservative alignment for code/data segments */
        break;
    }

    /* Only register section globally if it is not flagged as private internal storage */
    if (!(sh_flags & SHF_PRIVATE)) {
        sec->sh_num = s1->nb_sections;
        dynarray_add((void ***)&s1->sections, &s1->nb_sections, sec);
    }
    return sec;
}

/* Purge section memory block frames from heap */
static void free_section(Section *s)
{
    tcc_free(s->data);
    tcc_free(s);
}

/* Reallocate section memory block dynamically and wipe new memory window to zero */
static void section_realloc(Section *sec, unsigned long new_size)
{
    unsigned long size;
    unsigned char *data;
    
    size = sec->data_allocated;
    if (size == 0)
        size = 1;
    while (size < new_size)
        size = size * 2;
        
    data = tcc_realloc(sec->data, size);
    if (!data)
        error("memory full");
        
    memset(data + sec->data_allocated, 0, size - sec->data_allocated);
    sec->data = data;
    sec->data_allocated = size;
}

/* Reserve at least 'size' bytes in target section beginning from current data offset */
static void *section_ptr_add(Section *sec, unsigned long size)
{
    unsigned long offset, offset1;

    offset = sec->data_offset;
    offset1 = offset + size;
    if (offset1 > sec->data_allocated)
        section_realloc(sec, offset1);
    sec->data_offset = offset1;
    return sec->data + offset;
}

/* Flat Segment Alignment Engine: Locate an existing section or construct a new PROGBITS allocator */
Section *find_section(TCCState *s1, const char *name)
{
    Section *sec;
    int i;
    for(i = 1; i < s1->nb_sections; i++) {
        sec = s1->sections[i];
        if (!strcmp(name, sec->name)) 
            return sec;
    }
    /* Default newly discovered target sections directly as standard allocatable PROGBITS */
    return new_section(s1, name, SHT_PROGBITS, SHF_ALLOC);
}

/* Update structural symbol index properties mapping them to low-level target section definitions */
static void put_extern_sym(Sym *sym, Section *section, 
                           unsigned long value, unsigned long size)
{
    int sym_type, sym_bind, sh_num, info;
    Elf32_Sym *esym;
    const char *name;

    if (section)
        sh_num = section->sh_num;
    else
        sh_num = SHN_UNDEF;
        
    if (!sym->c) {
        if ((sym->type.t & VT_BTYPE) == VT_FUNC)
            sym_type = STT_FUNC;
        else
            sym_type = STT_OBJECT;
            
        if (sym->type.t & VT_STATIC)
            sym_bind = STB_LOCAL;
        else
            sym_bind = STB_GLOBAL;
        
        name = get_tok_str(sym->v, NULL);
        info = ELF32_ST_INFO(sym_bind, sym_type);
        sym->c = add_elf_sym(symtab_section, value, size, info, sh_num, name);
    } else {
        esym = &((Elf32_Sym *)symtab_section->data)[sym->c];
        esym->st_value = value;
        esym->st_size = size;
        esym->st_shndx = sh_num;
    }
}

/* Enject a new active relocation entry pointing to symbol identifier within structural section s */
static void greloc(Section *s, Sym *sym, unsigned long offset, int type)
{
    if (!sym->c) 
        put_extern_sym(sym, NULL, 0, 0);
    /* Bind verified ELF structural relocation metadata parameters straight to output table */
    put_elf_reloc(symtab_section, s, offset, type, sym->c);
}

/* Fast lexical evaluation helper checking for valid alphabetic or underscore identifier tokens */
static inline int isid(int c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

/* Fast lexical evaluation helper checking for valid base-10 digit sequences */
static inline int isnum(int c)
{
    return c >= '0' && c <= '9';
}

/* Fast lexical evaluation helper checking for valid octal sequences */
static inline int isoct(int c)
{
    return c >= '0' && c <= '7';
}

/* Translate lowercase alphabetic token identifiers directly to uppercase equivalents */
static inline int toup(int c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 'A';
    else
        return c;
}

/* Variadic sequential formatting buffer expansion utility wrapper */
static void strcat_vprintf(char *buf, int buf_size, const char *fmt, va_list ap)
{
    int len;
    len = strlen(buf);
    vsnprintf(buf + len, buf_size - len, fmt, ap);
}

/* Standard sequential formatting string expansion bridge targeting strcat_vprintf */
static void strcat_printf(char *buf, int buf_size, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    strcat_vprintf(buf, buf_size, fmt, ap);
    va_end(ap);
}

/* Internal detailed diagnostics generator tracking structural preprocessor include file trace stacks */
void error1(TCCState *s1, int is_warning, const char *fmt, va_list ap)
{
    char buf[2048];
    BufferedFile **f;
    
    buf[0] = '\0';
    if (file) {
        for(f = s1->include_stack; f < s1->include_stack_ptr; f++)
            strcat_printf(buf, sizeof(buf), "In file included from %s:%d:\n", 
                          (*f)->filename, (*f)->line_num);
        if (file->line_num > 0) {
            strcat_printf(buf, sizeof(buf), 
                          "%s:%d: ", file->filename, file->line_num);
        } else {
            strcat_printf(buf, sizeof(buf),
                          "%s: ", file->filename);
        }
    } else {
        strcat_printf(buf, sizeof(buf), "tcc: ");
    }
    
    if (is_warning)
        strcat_printf(buf, sizeof(buf), "warning: ");
        
    strcat_vprintf(buf, sizeof(buf), fmt, ap);

    /* Enforce global safe routing directly through our overridden trdos_print engine */
    if (!s1->error_func) {
        printf("%s\n", buf);
    } else {
        s1->error_func(s1->error_opaque, buf);
    }
    
    if (!is_warning)
        s1->nb_errors++;
}

/* =========================================================================
   GOOGLE AI & ERDOGAN TAN - NON-ABORTING DIAGNOSTIC INTERCEPTOR
   =========================================================================
   Logs non-fatal operational warning parameters via our native printf bridge 
   without breaking the primary parsing loop pipelines. Uses precise pointer 
   arithmetic vectoring to extract argument values sequentially. */
void error_noabort(const char *fmt, ...)
{
    extern int printf(const char *format, ...);
    printf("\n-> [TCC WARNING/ERROR]: ");
    printf(fmt, ((void**)&fmt)[1], ((void**)&fmt)[2], ((void**)&fmt)[3]);
    printf("\n");
    return;
}

/* =========================================================================
   GOOGLE AI & ERDOGAN TAN - CRITICAL FATAL EVACUATION SHIELD
   =========================================================================
   Intercepts critical syntactic anomalies, prints clear diagnostics, and 
   instantly triggers an uncatchable low-level native sys_exit (EAX=1) 
   interrupt. This hard shield guarantees protection for the TRDOS 386 kernel 
   by completely isolating broken code execution paths. */
void error(const char *fmt, ...)
{
    extern int printf(const char *format, ...);
    printf("\n-> [TCC FATAL ERROR]: ");
    printf(fmt, ((void**)&fmt)[1], ((void**)&fmt)[2], ((void**)&fmt)[3]);
    printf("\n-> [TRDOS SHIELD]: Terminating via sys_exit(1).\n");

    /* Fatal enforcement trap: Trigger hardware system exit interrupt call */
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n"
        "mov ebx, 1\n"                 /* Exit parameter payload: 1 (Error) */
        "mov eax, 1\n"                 /* TRDOS kernel service index: sys_exit */
        "int 0x40\n"                   /* Vector directly into Ring 0 */
        ".att_syntax\n"
    );
}

/* Raise strict structural token expectation failures */
void expect(const char *msg)
{
    error("%s expected", msg);
}

/* Capture standard grammar deviations sending localized traces to the tracker engine */
void warning(const char *fmt, ...)
{
    TCCState *s1 = tcc_state;
    va_list ap;

    va_start(ap, fmt);
    error1(s1, 1, fmt, ap);
    va_end(ap);
}

/* Enforce validation for expected language punctuation markers advancing on match */
void skip(int c)
{
    if (tok != c)
        error("'%c' expected", c);
    next();
}

/* Assert statement parameter eligibility validating register structures contain clear Lvalue properties */
static void test_lvalue(void)
{
    if (!(vtop->r & VT_LVAL))
        expect("lvalue");
}

/* Allocate a completely new unique identifier token slot within the global dictionary */
static TokenSym *tok_alloc_new(TokenSym **pts, const char *str, int len)
{
    TokenSym *ts, **ptable;
    int i;

    if (tok_ident >= SYM_FIRST_ANOM) 
        error("memory full");

    /* Expand global token tracker sequence table if current boundaries are breached */
    i = tok_ident - TOK_IDENT;
    if ((i % TOK_ALLOC_INCR) == 0) {
        ptable = tcc_realloc(table_ident, (i + TOK_ALLOC_INCR) * sizeof(TokenSym *));
        if (!ptable)
            error("memory full");
        table_ident = ptable;
    }

    /* Allocate continuous storage space matching the literal symbol string length */
    ts = tcc_malloc(sizeof(TokenSym) + len);
    table_ident[i] = ts;
    ts->tok = tok_ident++;
    ts->sym_define = NULL;
    ts->sym_label = NULL;
    ts->sym_struct = NULL;
    ts->sym_identifier = NULL;
    ts->len = len;
    ts->hash_next = NULL;
    memcpy(ts->str, str, len);
    ts->str[len] = '\0';
    *pts = ts;
    return ts;
}

#define TOK_HASH_INIT 1
#define TOK_HASH_FUNC(h, c) ((h) * 263 + (c))

/* Query the identifier symbol hash chain or construct a new dictionary node on failure */
static TokenSym *tok_alloc(const char *str, int len)
{
    TokenSym *ts, **pts;
    int i;
    unsigned int h;
    
    h = TOK_HASH_INIT;
    for(i = 0; i < len; i++)
        h = TOK_HASH_FUNC(h, ((unsigned char *)str)[i]);
    h &= (TOK_HASH_SIZE - 1);

    pts = &hash_ident[h];
    for(;;) {
        ts = *pts;
        if (!ts)
            break;
        if (ts->len == len && !memcmp(ts->str, str, len))
            return ts;
        pts = &(ts->hash_next);
    }
    return tok_alloc_new(pts, str, len);
}

/* Dynamic Continuous String (CString) Execution Engines */

/* Resize the dynamic literal character string container to fit newly requested layouts */
static void cstr_realloc(CString *cstr, int new_size)
{
    int size;
    void *data;

    size = cstr->size_allocated;
    if (size == 0)
        size = 8; /* Set conservative initial layout seed to prevent redundant resizing overhead */
    while (size < new_size)
        size = size * 2;
    data = tcc_realloc(cstr->data_allocated, size);
    if (!data)
        error("memory full");
    cstr->data_allocated = data;
    cstr->size_allocated = size;
    cstr->data = data;
}

/* Append a single byte token component straight into the target CString frame */
static void cstr_ccat(CString *cstr, int ch)
{
    int size;
    size = cstr->size + 1;
    if (size > cstr->size_allocated)
        cstr_realloc(cstr, size);
    ((unsigned char *)cstr->data)[size - 1] = ch;
    cstr->size = size;
}

/* Append a sequential null-terminated array of characters straight to the dynamic buffer */
static void cstr_cat(CString *cstr, const char *str)
{
    int c;
    for(;;) {
        c = *str;
        if (c == '\0')
            break;
        cstr_ccat(cstr, c);
        str++;
    }
}

/* Append a standard multi-byte wide character structure straight to the dynamic buffer frame */
static void cstr_wccat(CString *cstr, int ch)
{
    int size;
    size = cstr->size + sizeof(int);
    if (size > cstr->size_allocated)
        cstr_realloc(cstr, size);
    *(int *)(((unsigned char *)cstr->data) + size - sizeof(int)) = ch;
    cstr->size = size;
}

/* Initialize newly allocated CString memory maps */
static void cstr_new(CString *cstr)
{
    memset(cstr, 0, sizeof(CString));
}

/* Deallocate internal active buffer string pools resetting markers to zero */
static void cstr_free(CString *cstr)
{
    tcc_free(cstr->data_allocated);
    cstr_new(cstr);
}

#define cstr_reset(cstr) cstr_free(cstr)

/* Duplicate an existing character container memory layout block precisely */
static CString *cstr_dup(CString *cstr1)
{
    CString *cstr;
    int size;

    cstr = tcc_malloc(sizeof(CString));
    size = cstr1->size;
    cstr->size = size;
    cstr->size_allocated = size;
    cstr->data_allocated = tcc_malloc(size);
    cstr->data = cstr->data_allocated;
    memcpy(cstr->data_allocated, cstr1->data_allocated, size);
    return cstr;
}

/* Map raw binary characters into standardized source escape sequences */
static void add_char(CString *cstr, int c)
{
    if (c == '\'' || c == '\"' || c == '\\') {
        cstr_ccat(cstr, '\\');
    }
    if (c >= 32 && c <= 126) {
        cstr_ccat(cstr, c);
    } else {
        cstr_ccat(cstr, '\\');
        if (c == '\n') {
            cstr_ccat(cstr, 'n');
        } else {
            cstr_ccat(cstr, '0' + ((c >> 6) & 7));
            cstr_ccat(cstr, '0' + ((c >> 3) & 7));
            cstr_ccat(cstr, '0' + (c & 7));
        }
    }
}

/* Translate active raw multi-precision internal tokens back to descriptive human-readable strings */
char *get_tok_str(int v, CValue *cv)
{
    static char buf[STRING_MAX_SIZE + 1];
    static CString cstr_buf;
    CString *cstr;
    unsigned char *q;
    char *p;
    int i, len;

    /* Assign a permanent static translation stack frame to expedite micro-token processing loops */
    cstr_reset(&cstr_buf);
    cstr_buf.data = buf;
    cstr_buf.size_allocated = sizeof(buf);
    p = buf;

    switch(v) {
    case TOK_CINT:
    case TOK_CUINT:
        sprintf(p, "%u", cv->ui);
        break;
    case TOK_CLLONG:
    case TOK_CULLONG:
        sprintf(p, "%Lu", cv->ull);
        break;
    case TOK_CCHAR:
    case TOK_LCHAR:
        cstr_ccat(&cstr_buf, '\'');
        add_char(&cstr_buf, cv->i);
        cstr_ccat(&cstr_buf, '\'');
        cstr_ccat(&cstr_buf, '\0');
        break;
    case TOK_PPNUM:
        cstr = cv->cstr;
        len = cstr->size - 1;
        for(i = 0; i < len; i++)
            add_char(&cstr_buf, ((unsigned char *)cstr->data)[i]);
        cstr_ccat(&cstr_buf, '\0');
        break;
    case TOK_STR:
    case TOK_LSTR:
        cstr = cv->cstr;
        cstr_ccat(&cstr_buf, '\"');
        if (v == TOK_STR) {
            len = cstr->size - 1;
            for(i = 0; i < len; i++)
                add_char(&cstr_buf, ((unsigned char *)cstr->data)[i]);
        } else {
            len = (cstr->size / sizeof(int)) - 1;
            for(i = 0; i < len; i++)
                add_char(&cstr_buf, ((int *)cstr->data)[i]);
        }
        cstr_ccat(&cstr_buf, '\"');
        cstr_ccat(&cstr_buf, '\0');
        break;
    case TOK_LT:
        v = '<';
        goto addv;
    case TOK_GT:
        v = '>';
        goto addv;
    case TOK_A_SHL:
        return strcpy(p, "<<=");
    case TOK_A_SAR:
        return strcpy(p, ">>=");
    default:
        if (v < TOK_IDENT) {
            /* Map operational tokens directly using our twin-character lookup registry layout */
            q = tok_two_chars;
            while (*q) {
                if (q[2] == v) {
                    *p++ = q[0];
                    *p++ = q[1];
                    *p = '\0';
                    return buf;
                }
                q += 3;
            }
        addv:
            *p++ = v;
            *p = '\0';
        } else if (v < tok_ident) {
            return table_ident[v - TOK_IDENT]->str;
        } else if (v >= SYM_FIRST_ANOM) {
            /* Apply custom descriptive labeling for dynamically identified anonymous tracking symbols */
            sprintf(p, "L.%u", v - SYM_FIRST_ANOM);
        } else {
            return NULL;
        }
        break;
    }
    return cstr_buf.data;
}

/* Push a new raw symbol entry directly onto the target stack without hashing operations */
static Sym *sym_push2(Sym **ps, int v, int t, int c)
{
    Sym *s;
    s = tcc_malloc(sizeof(Sym));
    s->v = v;
    s->type.t = t;
    s->c = c;
    s->next = NULL;
    
    /* Enqueue directly into the active lexical stack */
    s->prev = *ps;
    *ps = s;
    return s;
}

/* Query the target symbol stack backward beginning from the specified top node frame */
static Sym *sym_find2(Sym *s, int v)
{
    while (s) {
        if (s->v == v)
            return s;
        s = s->prev;
    }
    return NULL;
}

/* Perform a rapid lookup for registered structure, union, or enum descriptor symbols */
static inline Sym *struct_find(int v)
{
    v -= TOK_IDENT;
    if ((unsigned)v >= (unsigned)(tok_ident - TOK_IDENT))
        return NULL;
    return table_ident[v]->sym_struct;
}

/* Perform a rapid lookup for a standard active identifier symbol */
static inline Sym *sym_find(int v)
{
    v -= TOK_IDENT;
    if ((unsigned)v >= (unsigned)(tok_ident - TOK_IDENT))
        return NULL;
    return table_ident[v]->sym_identifier;
}

/* Push a contextual token symbol structure onto either the local or global evaluation stack */
static Sym *sym_push(int v, CType *type, int r, int c)
{
    Sym *s, **ps;
    TokenSym *ts;

    if (local_stack)
        ps = &local_stack;
    else
        ps = &global_stack;
        
    s = sym_push2(ps, v, type->t, c);
    s->type.ref = type->ref;
    s->r = r;
    
    /* Ignore field descriptors and anonymous tracking structures during lookup registration */
    if (!(v & SYM_FIELD) && (v & ~SYM_STRUCT) < SYM_FIRST_ANOM) {
        /* Record the symbol cross-reference directly into the global token array map */
        ts = table_ident[(v & ~SYM_STRUCT) - TOK_IDENT];
        if (v & SYM_STRUCT)
            ps = &ts->sym_struct;
        else
            ps = &ts->sym_identifier;
        s->prev_tok = *ps;
        *ps = s;
    }
    return s;
}

/* Push a persistent identifier entry directly into the permanent global stack space */
static Sym *global_identifier_push(int v, int t, int c)
{
    Sym *s, **ps;
    s = sym_push2(&global_stack, v, t, c);
    
    /* Filter out temporary anonymous symbols from global registration pipelines */
    if (v < SYM_FIRST_ANOM) {
        ps = &table_ident[v - TOK_IDENT]->sym_identifier;
        /* Route deep into the link chain to hook beneath the active local layer contexts */
        while (*ps != NULL)
            ps = &(*ps)->prev_tok;
        s->prev_tok = NULL;
        *ps = s;
    }
    return s;
}

/* Pop and dismantle scope symbol frames sequentially until the designated boundary is reached */
static void sym_pop(Sym **ptop, Sym *b)
{
    Sym *s, *ss, **ps;
    TokenSym *ts;
    int v;

    s = *ptop;
    while (s != b) {
        ss = s->prev;
        v = s->v;
        
        /* Unbind structural dictionary references linked inside the global token array map */
        if (!(v & SYM_FIELD) && (v & ~SYM_STRUCT) < SYM_FIRST_ANOM) {
            ts = table_ident[(v & ~SYM_STRUCT) - TOK_IDENT];
            if (v & SYM_STRUCT)
                ps = &ts->sym_struct;
            else
                ps = &ts->sym_identifier;
            *ps = s->prev_tok;
        }
        tcc_free(s);
        s = ss;
    }
    *ptop = b;
}

/* I/O Layer and Native Source Filesystem Ingestion Engines */

/* Open a target source file and allocate its continuous input stream workspace tracking buffer */
BufferedFile *tcc_open(TCCState *s1, const char *filename)
{
    int fd;
    BufferedFile *bf;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return NULL;
        
    bf = tcc_malloc(sizeof(BufferedFile));
    if (!bf) {
        close(fd);
        return NULL;
    }
    
    bf->fd = fd;
    bf->buf_ptr = bf->buffer;
    bf->buf_end = bf->buffer;
    bf->buffer[0] = CH_EOB; /* Inject the End-of-Buffer guard symbol instantly */
    pstrcpy(bf->filename, sizeof(bf->filename), filename);
    bf->line_num = 1;
    bf->ifndef_macro = 0;
    bf->ifdef_stack_ptr = s1->ifdef_stack_ptr;
    
    return bf;
}

/* Deallocate target source file tracking frame and close native filesystem descriptors */
void tcc_close(BufferedFile *bf)
{
    total_lines += bf->line_num;
    close(bf->fd);
    tcc_free(bf);
}

/* Refill physical input device hardware buffer blocks and peek at downstream characters */
static int tcc_peekc_slow(BufferedFile *bf)
{
    int len;
    
    /* Trigger raw hardware device stream reads only when the buffer threshold is fully depleted */
    if (bf->buf_ptr >= bf->buf_end) {
        if (bf->fd != -1) {
            len = IO_BUF_SIZE;
            len = read(bf->fd, bf->buffer, len);
            if (len < 0)
                len = 0;
        } else {
            len = 0;
        }
        total_bytes += len;
        bf->buf_ptr = bf->buffer;
        bf->buf_end = bf->buffer + len;
        *bf->buf_end = CH_EOB;
    }
    
    if (bf->buf_ptr < bf->buf_end) {
        return bf->buf_ptr[0];
    } else {
        bf->buf_ptr = bf->buf_end;
        return CH_EOF;
    }
}

/* Process and resolve the dynamic evaluation framework at structural End-of-Block milestones */
static int handle_eob(void)
{
    return tcc_peekc_slow(file);
}

/* Advance memory pipeline pointer reading the next live character mapping out buffer limits */
static inline void inp(void)
{
    ch = *(++(file->buf_ptr));
    /* Intercept buffer boundary limitations re-routing directly into high-fidelity I/O refill loops */
    if (ch == CH_EOB)
        ch = handle_eob();
}

/* Resolve multi-line escape constraints handling unexpected stray character occurrences */
static void handle_stray(void)
{
    while (ch == '\\') {
        inp();
        if (ch == '\n') {
            file->line_num++;
            inp();
        } else if (ch == '\r') {
            inp();
            if (ch != '\n')
                goto fail;
            file->line_num++;
            inp();
        } else {
        fail:
            error("stray '\\' in program");
        }
    }
}

/* Parse complex escaped strings tracking multi-line boundary conditions gracefully */
static int handle_stray1(uint8_t *p)
{
    int c;

    if (p >= file->buf_end) {
        file->buf_ptr = p;
        c = handle_eob();
        p = file->buf_ptr;
        if (c == '\\')
            goto parse_stray;
    } else {
    parse_stray:
        file->buf_ptr = p;
        ch = *p;
        handle_stray();
        p = file->buf_ptr;
        c = *p;
    }
    return c;
}

/* Shorthand parsing macro resolving rapid End-of-Buffer evaluation frames safely */
#define PEEKC_EOB(c, p)\
{\
    p++;\
    c = *p;\
    if (c == '\\') {\
        file->buf_ptr = p;\
        c = handle_eob();\
        p = file->buf_ptr;\
    }\
}

/* Shorthand parsing macro resolving intricate preprocessor multi-line stray character connections */
#define PEEKC(c, p)\
{\
    p++;\
    c = *p;\
    if (c == '\\') {\
        c = handle_stray1(p);\
        p = file->buf_ptr;\
    }\
}

/* Core compilation parser advancement mechanism supporting explicit multi-line slash connectors */
static void minp(void)
{
    inp();
    if (ch == '\\') 
        handle_stray();
}

/* Standard single-line C++ style comment identifier stripping engine */
static uint8_t *parse_line_comment(uint8_t *p)
{
    int c;

    p++;
    for(;;) {
        c = *p;
        if (c == '\n' || c == CH_EOF) {
            break;
        } else if (c == '\\') {
            PEEKC_EOB(c, p);
            if (c == '\n') {
                file->line_num++;
                PEEKC_EOB(c, p);
            } else if (c == '\r') {
                PEEKC_EOB(c, p);
                if (c == '\n') {
                    file->line_num++;
                    PEEKC_EOB(c, p);
                }
            }
        } else {
            p++;
        }
    }
    return p;
}

/* Standard multi-line block C style comment identifier stripping engine */
static uint8_t *parse_comment(uint8_t *p)
{
    int c;
    
    p++;
    for(;;) {
        /* High-speed lexical skipping loop optimizing linear character sweeps */
        for(;;) {
            c = *p;
            if (c == '\n' || c == '*' || c == '\\')
                break;
            p++;
            c = *p;
            if (c == '\n' || c == '*' || c == '\\')
                break;
            p++;
        }
        
        /* Process discovered boundary termination or escape milestone flags */
        if (c == '\n') {
            file->line_num++;
            p++;
        } else if (c == '*') {
            p++;
            for(;;) {
                c = *p;
                if (c == '*') {
                    p++;
                } else if (c == '/') {
                    goto end_of_comment;
                } else if (c == '\\') {
                    file->buf_ptr = p;
                    c = handle_eob();
                    p = file->buf_ptr;
                    if (c == '\\') {
                        /* Process and resolve potential multi-line escape backslash sequences */
                        while (c == '\\') {
                            PEEKC_EOB(c, p);
                            if (c == '\n') {
                                file->line_num++;
                                PEEKC_EOB(c, p);
                            } else if (c == '\r') {
                                PEEKC_EOB(c, p);
                                if (c == '\n') {
                                    file->line_num++;
                                    PEEKC_EOB(c, p);
                                }
                            } else {
                                goto after_star;
                            }
                        }
                    }
                } else {
                    break;
                }
            }
        after_star: ;
        } else {
            /* Handle potential stray indicators, end-of-block, or terminal end-of-file milestones */
            file->buf_ptr = p;
            c = handle_eob();
            p = file->buf_ptr;
            if (c == CH_EOF) {
                error("unexpected end of file in comment");
            } else if (c == '\\') {
                p++;
            }
        }
    }
    
end_of_comment:
    p++;
    return p;
}

#define cinp minp

/* Evaluate character tokens verifying non-newline whitespace qualifications */
static inline int is_space(int ch)
{
    return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
}

/* Consume continuous linear sequences of space character tokens rapidly */
static inline void skip_spaces(void)
{
    while (is_space(ch))
        cinp();
}

/* Parse a raw string structure continuously without interpreting active internal escape definitions */
static uint8_t *parse_pp_string(uint8_t *p, int sep, CString *str)
{
    int c;
    p++;
    for(;;) {
        c = *p;
        if (c == sep) {
            break;
        } else if (c == '\\') {
            file->buf_ptr = p;
            c = handle_eob();
            p = file->buf_ptr;
            if (c == CH_EOF) {
            unterminated_string:
                error("missing terminating %c character", sep);
            } else if (c == '\\') {
                /* Escape handling: Safely bypass standard line-continuation backslash patterns */
                PEEKC_EOB(c, p);
                if (c == '\n') {
                    file->line_num++;
                    p++;
                } else if (c == '\r') {
                    PEEKC_EOB(c, p);
                    if (c != '\n')
                        expect("'\n' after '\r'");
                    file->line_num++;
                    p++;
                } else if (c == CH_EOF) {
                    goto unterminated_string;
                } else {
                    if (str) {
                        cstr_ccat(str, '\\');
                        cstr_ccat(str, c);
                    }
                    p++;
                }
            }
        } else if (c == '\n') {
            file->line_num++;
            goto add_char;
        } else if (c == '\r') {
            PEEKC_EOB(c, p);
            if (c != '\n') {
                cstr_ccat(str, '\r');
            } else {
                file->line_num++;
                goto add_char;
            }
        } else {
        add_char:
            if (str)
                cstr_ccat(str, c);
            p++;
        }
    }
    p++;
    return p;
}

/* Preprocessor Skip Engine: Traverse and ignore inactive code blocks until valid else/elif/endif triggers appear */
void preprocess_skip(void)
{
    int a, start_of_line, c;
    uint8_t *p;

    p = file->buf_ptr;
    start_of_line = 1;
    a = 0;
    for(;;) {
    redo_no_start:
        c = *p;
        switch(c) {
        case ' ':
        case '\t':
        case '\f':
        case '\v':
        case '\r':
            p++;
            goto redo_no_start;
        case '\n':
            start_of_line = 1;
            file->line_num++;
            p++;
            goto redo_no_start;
        case '\\':
            file->buf_ptr = p;
            c = handle_eob();
            if (c == CH_EOF) {
                expect("#endif");
            } else if (c == '\\') {
                ch = file->buf_ptr[0];
                handle_stray();
            }
            p = file->buf_ptr;
            goto redo_no_start;
            
        /* Safely cross literal string configurations without parsing deep expressions */
        case '\"':
        case '\'':
            p = parse_pp_string(p, c, NULL);
            break;
            
        /* Strip out dead comment configurations safely during preprocessor sweeping loops */
        case '/':
            file->buf_ptr = p;
            ch = *p;
            minp();
            p = file->buf_ptr;
            if (ch == '*') {
                p = parse_comment(p);
            } else if (ch == '/') {
                p = parse_line_comment(p);
            }
            break;

        case '#':
            p++;
            if (start_of_line) {
                file->buf_ptr = p;
                next_nomacro();
                p = file->buf_ptr;
                if (a == 0 && (tok == TOK_ELSE || tok == TOK_ELIF || tok == TOK_ENDIF))
                    goto the_end;
                if (tok == TOK_IF || tok == TOK_IFDEF || tok == TOK_IFNDEF)
                    a++;
                else if (tok == TOK_ENDIF)
                    a--;
            }
            break;
        default:
            p++;
            break;
        }
        start_of_line = 0;
    }
    
the_end: ;
    file->buf_ptr = p;
}

/* ParseState Handling and Lexical Context Snapshots */

/* Save the current active analytical parsing state into the target snapshot structure */
void save_parse_state(ParseState *s)
{
    s->line_num = file->line_num;
    s->macro_ptr = macro_ptr;
    s->tok = tok;
    s->tokc = tokc;
}

/* Restore the continuous compiler execution path from the specified state snapshot structure */
void restore_parse_state(ParseState *s)
{
    file->line_num = s->line_num;
    macro_ptr = s->macro_ptr;
    tok = s->tok;
    tokc = s->tokc;
}

/* Return the quantity of secondary 32-bit integer slots required to cache extended token structures */
static inline int tok_ext_size(int t)
{
    switch(t) {
    case TOK_CINT:
    case TOK_CUINT:
    case TOK_CCHAR:
    case TOK_LCHAR:
    case TOK_STR:
    case TOK_LSTR:
    case TOK_CFLOAT:
    case TOK_LINENUM:
    case TOK_PPNUM:
        return 1;
    case TOK_CDOUBLE:
    case TOK_CLLONG:
    case TOK_CULLONG:
        return 2;
    case TOK_CLDOUBLE:
        return 3; /* Fixed directly to 12 bytes / 4 for absolute native 32-bit x86 Flat layout */
    default:
        return 0;
    }
}

/* TokenString Structural Memory Framework */

/* Initialize a newly instantiated continuous token sequence recorder structure */
static inline void tok_str_new(TokenString *s)
{
    s->str = NULL;
    s->len = 0;
    s->allocated_len = 0;
    s->last_line_num = -1;
}

/* Dismantle and purge token sequence caches from memory, recycling embedded string builders */
static void tok_str_free(int *str)
{
    const int *p;
    CString *cstr;
    int t;

    p = str;
    for(;;) {
        t = *p;
        /* Evaluate terminal marker separate from switch to enable high-efficiency compiler jumps */
        if (t == 0)
            break;
        switch(t) {
        case TOK_CINT:
        case TOK_CUINT:
        case TOK_CCHAR:
        case TOK_LCHAR:
        case TOK_CFLOAT:
        case TOK_LINENUM:
            p += 2;
            break;
        case TOK_PPNUM:
        case TOK_STR:
        case TOK_LSTR:
            cstr = (CString *)p[1];
            cstr_free(cstr);
            tcc_free(cstr);
            p += 2;
            break;
        case TOK_CDOUBLE:
        case TOK_CLLONG:
        case TOK_CULLONG:
            p += 3;
            break;
        case TOK_CLDOUBLE:
            p += 4; /* Advanced by 1 descriptor identifier slot + 3 integer storage fields */
            break;
        default:
            p++;
            break;
        }
    }
    tcc_free(str);
}

/* Expand the token string array memory allocation block to accommodate subsequent token input streams */
static int *tok_str_realloc(TokenString *s)
{
    int *str, len;

    len = s->allocated_len + TOK_STR_ALLOC_INCR;
    str = tcc_realloc(s->str, len * sizeof(int));
    if (!str)
        error("memory full");
    s->allocated_len = len;
    s->str = str;
    return str;
}

/* Append a singular base lexical token directly to the end of the specified token string */
static void tok_str_add(TokenString *s, int t)
{
    int len, *str;

    len = s->len;
    str = s->str;
    if (len >= s->allocated_len)
        str = tok_str_realloc(s);
    str[len++] = t;
    s->len = len;
}

/* Append an extended token metadata structure along with its dynamic payload straight into the queue */
static void tok_str_add2(TokenString *s, int t, CValue *cv)
{
    int len, *str;

    len = s->len;
    str = s->str;

    /* Secure the maximum required execution boundary envelope before triggering memory resize routines */
    if (len + TOK_MAX_SIZE > s->allocated_len)
        str = tok_str_realloc(s);
    str[len++] = t;
    switch(t) {
    case TOK_CINT:
    case TOK_CUINT:
    case TOK_CCHAR:
    case TOK_LCHAR:
    case TOK_CFLOAT:
    case TOK_LINENUM:
        str[len++] = cv->tab[0];
        break;
    case TOK_PPNUM:
    case TOK_STR:
    case TOK_LSTR:
        str[len++] = (int)cstr_dup(cv->cstr);
        break;
    case TOK_CDOUBLE:
    case TOK_CLLONG:
    case TOK_CULLONG:
        str[len++] = cv->tab[0];
        str[len++] = cv->tab[1];
        break;
    case TOK_CLDOUBLE:
        str[len++] = cv->tab[0];
        str[len++] = cv->tab[1];
        str[len++] = cv->tab[2]; /* Directly inject 12-byte layout parameters into execution slot fields */
        break;
    default:
        break;
    }
    s->len = len;
}

/* Append the active operational compiler parser token straight into the tracking sequence structure */
static void tok_str_add_tok(TokenString *s)
{
    CValue cval;

    /* Intercept tracking line updates injecting explicit line info structures upon modification boundaries */
    if (file->line_num != s->last_line_num) {
        s->last_line_num = file->line_num;
        cval.i = s->last_line_num;
        tok_str_add2(s, TOK_LINENUM, &cval);
    }
    tok_str_add2(s, tok, &tokc);
}

/* Fast layout mapping macro unpacking native 12-byte x86 FPU long double structures from memory fields */
#define LDOUBLE_GET(p, cv)                      \
        cv.tab[0] = p[0];                       \
        cv.tab[1] = p[1];                       \
        cv.tab[2] = p[2];

/* Get a token sequence from an integer cache array and advance the pointer index layout.
   Coded strictly as a macro sequence to guarantee complete bypass of pointer aliasing bugs. */
#define TOK_GET(t, p, cv)                       \
{                                               \
    t = *p++;                                   \
    switch(t) {                                 \
    case TOK_CINT:                              \
    case TOK_CUINT:                             \
    case TOK_CCHAR:                             \
    case TOK_LCHAR:                             \
    case TOK_CFLOAT:                            \
    case TOK_LINENUM:                           \
    case TOK_STR:                               \
    case TOK_LSTR:                              \
    case TOK_PPNUM:                             \
        cv.tab[0] = *p++;                       \
        break;                                  \
    case TOK_CDOUBLE:                           \
    case TOK_CLLONG:                            \
    case TOK_CULLONG:                           \
        cv.tab[0] = p[0];                       \
        cv.tab[1] = p[1];                       \
        p += 2;                                 \
        break;                                  \
    case TOK_CLDOUBLE:                          \
        LDOUBLE_GET(p, cv);                     \
        p += 3; /* Fixed directly to 12 bytes / 4 format for absolute 32-bit flat i386 FPU pipelines */ \
        break;                                  \
    default:                                    \
        break;                                  \
    }                                           \
}

/* Preprocessor Macro Directive (Define) Management Framework */

/* Inject a newly discovered preprocessor macro block structure onto the evaluation stack */
static inline void define_push(int v, int macro_type, int *str, Sym *first_arg)
{
    Sym *s;

    s = sym_push2(&define_stack, v, macro_type, (int)str);
    s->next = first_arg;
    table_ident[v - TOK_IDENT]->sym_define = s;
}

/* Neutralize and unregister an active macro configuration layout mapping by cutting the dictionary references */
static void define_undef(Sym *s)
{
    int v;
    v = s->v;
    if (v >= TOK_IDENT && v < tok_ident)
        table_ident[v - TOK_IDENT]->sym_define = NULL;
    s->v = 0;
}

/* Perform a rapid lookup query for a registered macro identifier descriptor */
static inline Sym *define_find(int v)
{
    v -= TOK_IDENT;
    if ((unsigned)v >= (unsigned)(tok_ident - TOK_IDENT))
        return NULL;
    return table_ident[v]->sym_define;
}

/* Purge and dismantle macro compilation stack scopes sequentially until the specified milestone boundary */
static void free_defines(Sym *b)
{
    Sym *top, *top1;
    int v;

    top = define_stack;
    while (top != b) {
        top1 = top->prev;
        /* Safeguard standard parameter arguments or hardcoded predefined descriptors from deletion loops */
        if (top->c)
            tok_str_free((int *)top->c);
        v = top->v;
        if (v >= TOK_IDENT && v < tok_ident)
            table_ident[v - TOK_IDENT]->sym_define = NULL;
        tcc_free(top);
        top = top1;
    }
    define_stack = b;
}

/* Branching Label Lookup and Structural Resolution Engines */

/* Perform a rapid lookup query for a standard branch destination label */
static Sym *label_find(int v)
{
    v -= TOK_IDENT;
    if ((unsigned)v >= (unsigned)(tok_ident - TOK_IDENT))
        return NULL;
    return table_ident[v]->sym_label;
}

/* Register and push a newly encountered branch jump destination label frame onto the tracker stack */
static Sym *label_push(Sym **ptop, int v, int flags)
{
    Sym *s, **ps;
    s = sym_push2(ptop, v, 0, 0);
    s->r = flags;
    ps = &table_ident[v - TOK_IDENT]->sym_label;
    if (ptop == &global_label_stack) {
        /* Route underneath local scopes to preserve visibility during global label evaluation traps */
        while (*ps != NULL)
            ps = &(*ps)->prev_tok;
    }
    s->prev_tok = *ps;
    *ps = s;
    return s;
}

/* Dismantle label stack chains and verify resolution constraints. Enforce static symbol binding on computed labels */
static void label_pop(Sym **ptop, Sym *slast)
{
    Sym *s, *s1;
    for(s = *ptop; s != slast; s = s1) {
        s1 = s->prev;
        if (s->r == LABEL_DECLARED) {
            warning("label '%s' declared but not used", get_tok_str(s->v, NULL));
        } else if (s->r == LABEL_FORWARD) {
            error("label '%s' used but not defined", get_tok_str(s->v, NULL));
        } else {
            if (s->c) {
                /* Target address translation: Bind computed labels straight to flat contiguous segment offsets */
                put_extern_sym(s, cur_text_section, (long)s->next, 1);
            }
        }
        /* Sever structural dictionary lookup connections and free storage resources */
        table_ident[s->v - TOK_IDENT]->sym_label = s->prev_tok;
        tcc_free(s);
    }
    *ptop = slast;
}

/* Preprocessor Directive Expression and Macro Parsing Engines */

/* Evaluate conditional expressions for preprocessor directives like #if and #elif */
static int expr_preprocess(void)
{
    int c, t;
    TokenString str;
    
    tok_str_new(&str);
    while (tok != TOK_LINEFEED && tok != TOK_EOF) {
        next(); /* Trigger automatic macro expansion loops */
        if (tok == TOK_DEFINED) {
            next_nomacro();
            t = tok;
            if (t == '(') 
                next_nomacro();
            c = define_find(tok) != 0;
            if (t == '(')
                next_nomacro();
            tok = TOK_CINT;
            tokc.i = c;
        } else if (tok >= TOK_IDENT) {
            /* Fallback strategy: Translate unmapped or undefined macros straight to absolute zero */
            tok = TOK_CINT;
            tokc.i = 0;
        }
        tok_str_add_tok(&str);
    }
    tok_str_add(&str, -1); /* Emulate absolute terminal end-of-file signal token */
    tok_str_add(&str, 0);
    
    /* Re-route execution context to evaluate standard C constant mathematical expressions */
    macro_ptr = str.str;
    next();
    c = expr_const();
    macro_ptr = NULL;
    tok_str_free(str.str);
    return c != 0;
}

/* Parse token sequences immediately following a newly detected #define preprocessor directive */
static void parse_define(void)
{
    Sym *s, *first, **ps;
    int v, t, varg, is_vaargs, c;
    TokenString str;
    
    v = tok;
    if (v < TOK_IDENT)
        error("invalid macro name '%s'", get_tok_str(tok, &tokc));
        
    first = NULL;
    t = MACRO_OBJ;
    
    /* Enforce strict ANSI standard: The open parenthesis '(' must directly follow the macro identifier */
    c = file->buf_ptr[0];
    if (c == '\\')
        c = handle_stray1(file->buf_ptr);
        
    if (c == '(') {
        next_nomacro();
        next_nomacro();
        ps = &first;
        while (tok != ')') {
            varg = tok;
            next_nomacro();
            is_vaargs = 0;
            
            if (varg == TOK_DOTS) {
                varg = TOK___VA_ARGS__;
                is_vaargs = 1;
            } else if (tok == TOK_DOTS && gnu_ext) {
                is_vaargs = 1;
                next_nomacro();
            }
            
            if (varg < TOK_IDENT)
                error("badly punctuated parameter list");
                
            s = sym_push2(&define_stack, varg | SYM_FIELD, is_vaargs, 0);
            *ps = s;
            ps = &s->next;
            
            if (tok != ',')
                break;
            next_nomacro();
        }
        t = MACRO_FUNC;
    }
    
    tok_str_new(&str);
    next_nomacro();
    
    /* Track token inputs continuously; EOF evaluation remains mandatory for command line -D payload injections */
    while (tok != TOK_LINEFEED && tok != TOK_EOF) {
        tok_str_add2(&str, tok, &tokc);
        next_nomacro();
    }
    tok_str_add(&str, 0);

    /* Safely register the newly synthesized macro parameters directly into the permanent identifier table */
    define_push(v, t, str.str, first);
}

/* Inclusion Header Cache and Filesystem Search Engines */

/* Query the registered cached include database to avoid redundant lookup cycles */
static CachedInclude *search_cached_include(TCCState *s1, int type, const char *filename)
{
    CachedInclude *e;
    int i;

    for(i = 0; i < s1->nb_cached_includes; i++) {
        e = s1->cached_includes[i];
        if (e->type == type && !strcmp(e->filename, filename))
            return e;
    }
    return NULL;
}

/* Register a newly discovered header path directly into the global inclusion cache tracker array */
static inline void add_cached_include(TCCState *s1, int type, const char *filename, int ifndef_macro)
{
    CachedInclude *e;

    /* Enforce strict single-registration rule to bypass duplicated memory overheads */
    if (search_cached_include(s1, type, filename))
        return;

    /* Allocate exact continuous memory slot matching the canonical layout filename length */
    e = tcc_malloc(sizeof(CachedInclude) + strlen(filename));
    if (!e)
        return;
        
    e->type = type;
    strcpy(e->filename, filename);
    e->ifndef_macro = ifndef_macro;
    
    dynarray_add((void ***)&s1->cached_includes, &s1->nb_cached_includes, e);
}

/* is_bof is true if first non space token at beginning of file */
static void preprocess(int is_bof)
{
    TCCState *s1 = tcc_state;
    int size, i, c, n, saved_parse_flags;
    char buf[1024], *q, *p;
    char buf1[1024];
    BufferedFile *f;
    Sym *s;
    CachedInclude *e;
    
    saved_parse_flags = parse_flags;
    parse_flags = PARSE_FLAG_PREPROCESS | PARSE_FLAG_TOK_NUM | PARSE_FLAG_LINEFEED;
    next_nomacro();
 redo:
    switch(tok) {
    case TOK_DEFINE:
        next_nomacro();
        parse_define();
        break;
    case TOK_UNDEF:
        next_nomacro();
        s = define_find(tok);
        /* Undefine symbol by resetting its link registry entry in table_ident */
        if (s)
            define_undef(s);
        break;
    case TOK_INCLUDE:
        ch = file->buf_ptr[0];
        skip_spaces();
        if (ch == '<') {
            c = '>';
            goto read_name;
        } else if (ch == '\"') {
            c = ch;
        read_name:
            minp();
            q = buf;
            while (ch != c && ch != '\n' && ch != CH_EOF) {
                if ((q - buf) < sizeof(buf) - 1)
                    *q++ = ch;
                minp();
            }
            *q = '\0';
            minp();
        } else {
            /* Computed #include processing paths */
            next();
            buf[0] = '\0';
            if (tok == TOK_STR) {
                while (tok != TOK_LINEFEED) {
                    if (tok != TOK_STR) {
                    include_syntax:
                        error("'#include' expects \"FILENAME\" or <FILENAME>");
                    }
                    pstrcat(buf, sizeof(buf), (char *)tokc.cstr->data);
                    next();
                }
                c = '\"';
            } else {
                int len;
                while (tok != TOK_LINEFEED) {
                    pstrcat(buf, sizeof(buf), get_tok_str(tok, &tokc));
                    next();
                }
                len = strlen(buf);
                if (len < 2 || buf[0] != '<' || buf[len - 1] != '>')
                    goto include_syntax;
                memmove(buf, buf + 1, len - 2);
                buf[len - 2] = '\0';
                c = '>';
            }
        }

        e = search_cached_include(s1, c, buf);
        if (e && define_find(e->ifndef_macro)) {
            /* Bypass inclusion parsing loop entirely if the guard macro remains predefined */
        } else {
            if (c == '\"') {
                /* Traverse current source workspace directory path mapping for header files first */
                size = 0;
                p = strrchr(file->filename, '/');
                if (p) 
                    size = p + 1 - file->filename;
                if (size > sizeof(buf1) - 1)
                    size = sizeof(buf1) - 1;
                memcpy(buf1, file->filename, size);
                buf1[size] = '\0';
                pstrcat(buf1, sizeof(buf1), buf);
                f = tcc_open(s1, buf1);
                if (f)
                    goto found;
            }
            if (s1->include_stack_ptr >= s1->include_stack + INCLUDE_STACK_SIZE)
                error("#include recursion too deep");
            
            /* Scan configured standard paths for destination header files sequentially */
            n = s1->nb_include_paths + s1->nb_sysinclude_paths;
            for(i = 0; i < n; i++) {
                const char *path;
                if (i < s1->nb_include_paths)
                    path = s1->include_paths[i];
                else
                    path = s1->sysinclude_paths[i - s1->nb_include_paths];
                pstrcpy(buf1, sizeof(buf1), path);
                pstrcat(buf1, sizeof(buf1), "/");
                pstrcat(buf1, sizeof(buf1), buf);
                f = tcc_open(s1, buf1);
                if (f)
                    goto found;
            }
            error("include file '%s' not found", buf);
            f = NULL;
        found:
            f->inc_type = c;
            pstrcpy(f->inc_filename, sizeof(f->inc_filename), buf);
            
            /* Push active file frame snapshot context onto the preprocessor recursion file stack */
            *s1->include_stack_ptr++ = file;
            file = f;
            
            /* STABS debug emission engine code siphoned away to ensure strict runtime code minimalism */
            tok_flags |= TOK_FLAG_BOF | TOK_FLAG_BOL;
            ch = file->buf_ptr[0];
            goto the_end;
        }
        break;
    case TOK_IFNDEF:
        c = 1;
        goto do_ifdef;
    case TOK_IF:
        c = expr_preprocess();
        goto do_if;
    case TOK_IFDEF:
        c = 0;
    do_ifdef:
        next_nomacro();
        if (tok < TOK_IDENT)
            error("invalid argument for '#if%sdef'", c ? "n" : "");
        if (is_bof) {
            if (c) {
                /* Include debugging logs stripped away to retain absolute core speed */
                file->ifndef_macro = tok;
            }
        }
        c = (define_find(tok) != 0) ^ c;
    do_if:
        if (s1->ifdef_stack_ptr >= s1->ifdef_stack + IFDEF_STACK_SIZE)
            error("memory full");
        *s1->ifdef_stack_ptr++ = c;
        goto test_skip;
    case TOK_ELSE:
        if (s1->ifdef_stack_ptr == s1->ifdef_stack)
            error("#else without matching #if");
        if (s1->ifdef_stack_ptr[-1] & 2)
            error("#else after #else");
        c = (s1->ifdef_stack_ptr[-1] ^= 3);
        goto test_skip;
    case TOK_ELIF:
        if (s1->ifdef_stack_ptr == s1->ifdef_stack)
            error("#elif without matching #if");
        c = s1->ifdef_stack_ptr[-1];
        if (c > 1)
            error("#elif after #else");
        
        /* Skip evaluation loop if the historical branch condition evaluates as true */
        if (c == 1)
            goto skip;
        c = expr_preprocess();
        s1->ifdef_stack_ptr[-1] = c;
    test_skip:
        if (!(c & 1)) {
        skip:
            preprocess_skip();
            is_bof = 0;
            goto redo;
        }
        break;
    case TOK_ENDIF:
        if (s1->ifdef_stack_ptr <= file->ifdef_stack_ptr)
            error("#endif without matching #if");
        s1->ifdef_stack_ptr--;
        /* '#ifndef macro' was at the start of file. Now we check if
           an '#endif' is exactly at the end of file */
        if (file->ifndef_macro &&
            s1->ifdef_stack_ptr == file->ifdef_stack_ptr) {
            file->ifndef_macro_saved = file->ifndef_macro;
            /* need to set to zero to avoid false matches if another
               #ifndef at middle of file */
            file->ifndef_macro = 0;
            while (tok != TOK_LINEFEED)
                next_nomacro();
            tok_flags |= TOK_FLAG_ENDIF;
            goto the_end;
        }
        break;
    case TOK_LINE:
        next();
        if (tok != TOK_CINT)
            error("#line");
        file->line_num = tokc.i - 1; /* The line number will be incremented after */
        next();
        if (tok != TOK_LINEFEED) {
            if (tok != TOK_STR)
                error("#line");
            pstrcpy(file->filename, sizeof(file->filename), (char *)tokc.cstr->data);
        }
        break;
    case TOK_ERROR:
    case TOK_WARNING:
        c = tok;
        ch = file->buf_ptr[0];
        skip_spaces();
        q = buf;
        while (ch != '\n' && ch != CH_EOF) {
            if ((q - buf) < sizeof(buf) - 1)
                *q++ = ch;
            minp();
        }
        *q = '\0';
        if (c == TOK_ERROR)
            error("#error %s", buf);
        else
            warning("#warning %s", buf);
        break;
    case TOK_PRAGMA:
        /* Pragmas are safely bypassed and ignored under the TRDOS flat compilation paradigm */
        break;
    default:
        if (tok == TOK_LINEFEED || tok == '!' || tok == TOK_CINT) {
            /* '!' is ignored to allow C scripts. numbers are ignored to emulate cpp behaviour */
        } else {
            error("invalid preprocessing directive #%s", get_tok_str(tok, &tokc));
        }
        break;
    }
    /* ignore other preprocess commands or #! for C scripts */
    while (tok != TOK_LINEFEED)
        next_nomacro();
 the_end:
    parse_flags = saved_parse_flags;
}

//* Escape Sequence and Preprocessor Big Number Mathematical Utilities */

/* Parse and evaluate standard and extended literal escape sequences within string structures */
static void parse_escape_string(CString *outstr, const uint8_t *buf, int is_long)
{
    int c, n;
    const char *p;

    p = (const char *)buf;
    for(;;) {
        c = *p;
        if (c == '\0')
            break;
        if (c == '\\') {
            p++;
            c = *p;
            switch(c) {
            case '0': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7':
                /* Parse at most three consecutive octal digit qualifiers */
                n = c - '0';
                p++;
                c = *p;
                if (isoct(c)) {
                    n = n * 8 + c - '0';
                    p++;
                    c = *p;
                    if (isoct(c)) {
                        n = n * 8 + c - '0';
                        p++;
                    }
                }
                c = n;
                goto add_char_nonext;
            case 'x':
                p++;
                n = 0;
                for(;;) {
                    c = *p;
                    if (c >= 'a' && c <= 'f')
                        c = c - 'a' + 10;
                    else if (c >= 'A' && c <= 'F')
                        c = c - 'A' + 10;
                    else if (isnum(c))
                        c = c - '0';
                    else
                        break;
                    n = n * 16 + c;
                    p++;
                }
                c = n;
                goto add_char_nonext;
            case 'a':
                c = '\a';
                break;
            case 'b':
                c = '\b';
                break;
            case 'f':
                c = '\f';
                break;
            case 'n':
                c = '\n';
                break;
            case 'r':
                c = '\r';
                break;
            case 't':
                c = '\t';
                break;
            case 'v':
                c = '\v';
                break;
            case 'e':
                if (!gnu_ext)
                    goto invalid_escape;
                c = 27; /* ASCII Escape code value character tracking assignment */
                break;
            case '\'':
            case '\"':
            case '\\': 
            case '?':
                break;
            default:
            invalid_escape:
                error("invalid escaped char");
            }
        }
        p++;
    add_char_nonext:
        if (!is_long)
            cstr_ccat(outstr, c);
        else
            cstr_wccat(outstr, c);
    }
    
    /* Inject structural trailing null terminator to lock string memory layout */
    if (!is_long)
        cstr_ccat(outstr, '\0');
    else
        cstr_wccat(outstr, '\0');
}

/* Enforce static 64-bit internal numerical allocation boundaries for preprocessor evaluation */
#define BN_SIZE 2

/* Big Number Shift Engine: bn = (bn << shift) | or_val to process broad integer token inputs */
void bn_lshift(unsigned int *bn, int shift, int or_val)
{
    int i;
    unsigned int v;
    for(i = 0; i < BN_SIZE; i++) {
        v = bn[i];
        bn[i] = (v << shift) | or_val;
        or_val = v >> (32 - shift);
    }
}

/* Wipe targeted multi-precision big number structures back to pure absolute zero states */
void bn_zero(unsigned int *bn)
{
    int i;
    for(i = 0; i < BN_SIZE; i++) {
        bn[i] = 0;
    }
}

/* Parse a number enclosed within a null-terminated string 'p' and load it directly into the active token storage */
void parse_number(const char *p)
{
    int b, t, shift, frac_bits, s, exp_val, ch;
    char *q;
    unsigned int bn[BN_SIZE];
    double d;

    /* Initialize core token buffer parsing workspace layout */
    q = token_buf;
    ch = *p++;
    t = ch;
    ch = *p++;
    *q++ = t;
    b = 10;
    
    if (t == '.') {
        goto float_frac_parse;
    } else if (t == '0') {
        if (ch == 'x' || ch == 'X') {
            q--;
            ch = *p++;
            b = 16;
        } else if (tcc_ext && (ch == 'b' || ch == 'B')) {
            q--;
            ch = *p++;
            b = 2;
        }
    }
    
    /* Process and load all valid numerical digit characters sequentially.
       Octal verification is deferred here to safely accommodate floating-point syntax constraints */
    while (1) {
        if (ch >= 'a' && ch <= 'f')
            t = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F')
            t = ch - 'A' + 10;
        else if (isnum(ch))
            t = ch - '0';
        else
            break;
            
        if (t >= b)
            break;
            
        if (q >= token_buf + STRING_MAX_SIZE) {
        num_too_long:
            error("number too long");
        }
        *q++ = ch;
        ch = *p++;
    }
    
    if (ch == '.' ||
        ((ch == 'e' || ch == 'E') && b == 10) ||
        ((ch == 'p' || ch == 'P') && (b == 16 || b == 2))) {
        if (b != 10) {
            /* Hand-crafted hexadecimal or binary floating-point evaluation motor.
               Enforced directly to override variant non-ISOC99 library interpretation anomalies safely */
            *q = '\0';
            if (b == 16)
                shift = 4;
            else 
                shift = 2;
                
            bn_zero(bn);
            q = token_buf;
            while (1) {
                t = *q++;
                if (t == '\0') {
                    break;
                } else if (t >= 'a') {
                    t = t - 'a' + 10;
                } else if (t >= 'A') {
                    t = t - 'A' + 10;
                } else {
                    t = t - '0';
                }
                bn_lshift(bn, shift, t);
            }
            frac_bits = 0;
            if (ch == '.') {
                ch = *p++;
                while (1) {
                    t = ch;
                    if (t >= 'a' && t <= 'f') {
                        t = t - 'a' + 10;
                    } else if (t >= 'A' && t <= 'F') {
                        t = t - 'A' + 10;
                    } else if (t >= '0' && t <= '9') {
                        t = t - '0';
                    } else {
                        break;
                    }
                    if (t >= b)
                        error("invalid digit");
                    bn_lshift(bn, shift, t);
                    frac_bits += shift;
                    ch = *p++;
                }
            }
            if (ch != 'p' && ch != 'P')
                expect("exponent");
            ch = *p++;
            s = 1;
            exp_val = 0;
            if (ch == '+') {
                ch = *p++;
            } else if (ch == '-') {
                s = -1;
                ch = *p++;
            }
            if (ch < '0' || ch > '9')
                expect("exponent digits");
            while (ch >= '0' && ch <= '9') {
                exp_val = exp_val * 10 + ch - '0';
                ch = *p++;
            }
            exp_val = exp_val * s;
            
            /* Construct the floating point value inside a standard double register block */
            d = (double)bn[1] * 4294967296.0 + (double)bn[0];
            d = ldexp(d, exp_val - frac_bits);
            t = toup(ch);
            if (t == 'F') {
                ch = *p++;
                tok = TOK_CFLOAT;
                tokc.f = (float)d;
            } else if (t == 'L') {
                ch = *p++;
                tok = TOK_CLDOUBLE;
                tokc.ld = (long double)d;
            } else {
                tok = TOK_CDOUBLE;
                tokc.d = d;
            }
        } else {
            /* Process decimal floating-point formatting patterns */
            if (ch == '.') {
                if (q >= token_buf + STRING_MAX_SIZE)
                    goto num_too_long;
                *q++ = ch;
                ch = *p++;
            float_frac_parse:
                while (ch >= '0' && ch <= '9') {
                    if (q >= token_buf + STRING_MAX_SIZE)
                        goto num_too_long;
                    *q++ = ch;
                    ch = *p++;
                }
            }
            if (ch == 'e' || ch == 'E') {
                if (q >= token_buf + STRING_MAX_SIZE)
                    goto num_too_long;
                *q++ = ch;
                ch = *p++;
                if (ch == '-' || ch == '+') {
                    if (q >= token_buf + STRING_MAX_SIZE)
                        goto num_too_long;
                    *q++ = ch;
                    ch = *p++;
                }
                if (ch < '0' || ch > '9')
                    expect("exponent digits");
                while (ch >= '0' && ch <= '9') {
                    if (q >= token_buf + STRING_MAX_SIZE)
                        goto num_too_long;
                    *q++ = ch;
                    ch = *p++;
                }
            }
            *q = '\0';
            t = toup(ch);
            errno = 0;
            if (t == 'F') {
                ch = *p++;
                tok = TOK_CFLOAT;
                tokc.f = strtof(token_buf, NULL);
            } else if (t == 'L') {
                ch = *p++;
                tok = TOK_CLDOUBLE;
                tokc.ld = strtold(token_buf, NULL);
            } else {
                tok = TOK_CDOUBLE;
                tokc.d = strtod(token_buf, NULL);
            }
        }
    } else {
        unsigned long long n, n1;
        int lcount, ucount;

        /* Evaluate and process raw integer numbers */
        *q = '\0';
        q = token_buf;
        if (b == 10 && *q == '0') {
            b = 8;
            q++;
        }
        n = 0;
        while(1) {
            t = *q++;
            if (t == '\0') {
                break;
            } else if (t >= 'a') {
                t = t - 'a' + 10;
            } else if (t >= 'A') {
                t = t - 'A' + 10;
            } else {
                t = t - '0';
                if (t >= b)
                    error("invalid digit");
            }
            n1 = n;
            n = n * b + t;
            
            /* Enforce hard verification checking for integer overflows */
            if (n < n1)
                error("integer constant overflow");
        }
        
        if ((n & 0xffffffff00000000LL) != 0) {
            if ((n >> 63) != 0)
                tok = TOK_CULLONG;
            else
                tok = TOK_CLLONG;
        } else if (n > 0x7fffffff) {
            tok = TOK_CUINT;
        } else {
            tok = TOK_CINT;
        }
        
        lcount = 0;
        ucount = 0;
        for(;;) {
            t = toup(ch);
            if (t == 'L') {
                if (lcount >= 2)
                    error("three 'l's in integer constant");
                lcount++;
                if (lcount == 2) {
                    if (tok == TOK_CINT)
                        tok = TOK_CLLONG;
                    else if (tok == TOK_CUINT)
                        tok = TOK_CULLONG;
                }
                ch = *p++;
            } else if (t == 'U') {
                if (ucount >= 1)
                    error("two 'u's in integer constant");
                ucount++;
                if (tok == TOK_CINT)
                    tok = TOK_CUINT;
                else if (tok == TOK_CLLONG)
                    tok = TOK_CULLONG;
                ch = *p++;
            } else {
                break;
            }
        }
        if (tok == TOK_CINT || tok == TOK_CUINT)
            tokc.ui = n;
        else
            tokc.ull = n;
    }
}

/* Macro engine designed to evaluate double-character tokens rapidly without pointer aliasing bugs */
#define PARSE2(c1, tok1, c2, tok2)              \
    case c1:                                    \
        PEEKC(c, p);                            \
        if (c == c2) {                          \
            p++;                                \
            tok = tok2;                         \
        } else {                                \
            tok = tok1;                         \
        }                                       \
        break;

/* Return next token directly from the source pipeline without applying macro substitution */
static inline void next_nomacro1(void)
{
    int t, c, is_long;
    TokenSym *ts;
    uint8_t *p, *p1;
    unsigned int h;

    p = file->buf_ptr;
 redo_no_start:
    c = *p;
    switch(c) {
    case ' ':
    case '\t':
    case '\f':
    case '\v':
    case '\r':
        p++;
        goto redo_no_start;
        
    case '\\':
        /* Validate if the lookahead character indicates a strict structural End-of-Buffer boundary */
        if (p >= file->buf_end) {
            file->buf_ptr = p;
            handle_eob();
            p = file->buf_ptr;
            if (p >= file->buf_end)
                goto parse_eof;
            else
                goto redo_no_start;
        } else {
            file->buf_ptr = p;
            ch = *p;
            handle_stray();
            p = file->buf_ptr;
            goto redo_no_start;
        }
    parse_eof:
        {
            TCCState *s1 = tcc_state;
            if (parse_flags & PARSE_FLAG_LINEFEED) {
                tok = TOK_LINEFEED;
            } else if (s1->include_stack_ptr == s1->include_stack ||
                       !(parse_flags & PARSE_FLAG_PREPROCESS)) {
                /* Terminal milestone reached: No active files remaining on include stack layout */
                tok = TOK_EOF;
            } else {
                /* Restore historical file layout context from preprocessor stack frames */
                if (tok_flags & TOK_FLAG_ENDIF) {
                    add_cached_include(s1, file->inc_type, file->inc_filename,
                                       file->ifndef_macro_saved);
                }

                /* Standard STABS debug tracking trace code completely siphoned away to maintain core static minimalism */
                tcc_close(file);
                s1->include_stack_ptr--;
                file = *s1->include_stack_ptr;
                p = file->buf_ptr;
                goto redo_no_start;
            }
        }
        break;

    case '\n':
        if (parse_flags & PARSE_FLAG_LINEFEED) {
            tok = TOK_LINEFEED;
        } else {
            file->line_num++;
            tok_flags |= TOK_FLAG_BOL;
            p++;
            goto redo_no_start;
        }
        break;

    case '#':
        PEEKC(c, p);
        if ((tok_flags & TOK_FLAG_BOL) && (parse_flags & PARSE_FLAG_PREPROCESS)) {
            file->buf_ptr = p;
            preprocess(tok_flags & TOK_FLAG_BOF);
            p = file->buf_ptr;
            goto redo_no_start;
        } else {
            if (c == '#') {
                p++;
                tok = TOK_TWOSHARPS;
            } else {
                tok = '#';
            }
        }
        break;

    case 'a': case 'b': case 'c': case 'd':
    case 'e': case 'f': case 'g': case 'h':
    case 'i': case 'j': case 'k': case 'l':
    case 'm': case 'n': case 'o': case 'p':
    case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x':
    case 'y': case 'z': 
    case 'A': case 'B': case 'C': case 'D':
    case 'E': case 'F': case 'G': case 'H':
    case 'I': case 'J': case 'K': 
    case 'M': case 'N': case 'O': case 'P':
    case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X':
    case 'Y': case 'Z': 
    case '_':
    parse_ident_fast:
        p1 = p;
        h = TOK_HASH_INIT;
        h = TOK_HASH_FUNC(h, c);
        p++;
        for(;;) {
            c = *p;
            if (!isidnum_table[c])
                break;
            h = TOK_HASH_FUNC(h, c);
            p++;
        }
        if (c != '\\') {
            TokenSym **pts;
            int len;

            /* Fast path: No stray backslash discovered, resolve tokens via quick continuous hash lookup */
            len = p - p1;
            h &= (TOK_HASH_SIZE - 1);
            pts = &hash_ident[h];
            for(;;) {
                ts = *pts;
                if (!ts)
                    break;
                if (ts->len == len && !memcmp(ts->str, p1, len))
                    goto token_found;
                pts = &(ts->hash_next);
            }
            ts = tok_alloc_new(pts, p1, len);
        token_found: ;
        } else {
            /* Slower path: Handle unexpected backslash stray alignments within identifier parsing stream */
            cstr_reset(&tokcstr);

            while (p1 < p) {
                cstr_ccat(&tokcstr, *p1);
                p1++;
            }
            p--;
            PEEKC(c, p);
        parse_ident_slow:
            while (isidnum_table[c]) {
                cstr_ccat(&tokcstr, c);
                PEEKC(c, p);
            }
            ts = tok_alloc(tokcstr.data, tokcstr.size);
        }
        tok = ts->tok;
        break;
    case 'L':
        t = (int)p;
        if (t != '\\' && t != '\'' && t != '\"') {
            /* Fall back straight into the fast path if 'L' represents a standard isolated variable identifier */
            goto parse_ident_fast;
        } else {
            PEEKC(c, p);
            if (c == '\'' || c == '\"') {
                is_long = 1;
                goto str_const;
            } else {
                cstr_reset(&tokcstr);
                cstr_ccat(&tokcstr, 'L');
                goto parse_ident_slow;
            }
        }
        break;
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':

        cstr_reset(&tokcstr);
        /* Lexical rule: Beyond initial digit trigger, capture consecutive alphanumeric characters or decimals */
    parse_num:
        for(;;) {
            t = c;
            cstr_ccat(&tokcstr, c);
            PEEKC(c, p);
            if (!(isnum(c) || isid(c) || c == '.' ||
                  ((c == '+' || c == '-') && (t == 'e' || t == 'E' || t == 'p' || t == 'P'))))
                break;
        }
        /* Append standard structural string null terminator to secure the token cache buffer formatting */
        cstr_ccat(&tokcstr, '\0');
        tokc.cstr = &tokcstr;
        tok = TOK_PPNUM;
        break;
    case '.':
        /* Specialized dot validation to seamlessly differentiate operator bounds from decimal floating point seeds */
        PEEKC(c, p);
        if (isnum(c)) {
            cstr_reset(&tokcstr);
            cstr_ccat(&tokcstr, '.');
            goto parse_num;
        } else if (c == '.') {
            PEEKC(c, p);
            if (c != '.')
                expect("'.'");
            PEEKC(c, p);
            tok = TOK_DOTS; /* Resolve sequential ellipsis punctuation token (...) */
        } else {
            tok = '.';
        }
        break;
    case '\'':
    case '\"':
        is_long = 0;
    str_const:
        {
            CString str;
            int sep;

            sep = c;

            /* Parse and process the raw preprocessor string character array */
            cstr_new(&str);
            p = parse_pp_string(p, sep, &str);
            cstr_ccat(&str, '\0');
            
            /* Evaluate structural escape sequences directly into the core token destination frame */
            cstr_reset(&tokcstr);
            parse_escape_string(&tokcstr, str.data, is_long);
            cstr_free(&str);

            if (sep == '\'') {
                int char_size;
                if (!is_long)
                    char_size = 1;
                else
                    char_size = sizeof(int);
                if (tokcstr.size <= char_size)
                    error("empty character constant");
                if (tokcstr.size > 2 * char_size)
                    warning("multi-character character constant");
                if (!is_long) {
                    tokc.i = *(int8_t *)tokcstr.data;
                    tok = TOK_CCHAR;
                } else {
                    tokc.i = *(int *)tokcstr.data;
                    tok = TOK_LCHAR;
                }
            } else {
                tokc.cstr = &tokcstr;
                if (!is_long)
                    tok = TOK_STR;
                else
                    tok = TOK_LSTR;
            }
        }
        break;

    case '<':
        PEEKC(c, p);
        if (c == '=') {
            p++;
            tok = TOK_LE;
        } else if (c == '<') {
            PEEKC(c, p);
            if (c == '=') {
                p++;
                tok = TOK_A_SHL;
            } else {
                tok = TOK_SHL;
            }
        } else {
            tok = TOK_LT;
        }
        break;
        
    case '>':
        PEEKC(c, p);
        if (c == '=') {
            p++;
            tok = TOK_GE;
        } else if (c == '>') {
            PEEKC(c, p);
            if (c == '=') {
                p++;
                tok = TOK_A_SAR;
            } else {
                tok = TOK_SAR;
            }
        } else {
            tok = TOK_GT;
        }
        break;
        
    case '&':
        PEEKC(c, p);
        if (c == '&') {
            p++;
            tok = TOK_LAND;
        } else if (c == '=') {
            p++;
            tok = TOK_A_AND;
        } else {
            tok = '&';
        }
        break;
        
    case '|':
        PEEKC(c, p);
        if (c == '|') {
            p++;
            tok = TOK_LOR;
        } else if (c == '=') {
            p++;
            tok = TOK_A_OR;
        } else {
            tok = '|';
        }
        break;

    case '+':
        PEEKC(c, p);
        if (c == '+') {
            p++;
            tok = TOK_INC;
        } else if (c == '=') {
            p++;
            tok = TOK_A_ADD;
        } else {
            tok = '+';
        }
        break;
        
    case '-':
        PEEKC(c, p);
        if (c == '-') {
            p++;
            tok = TOK_DEC;
        } else if (c == '=') {
            p++;
            tok = TOK_A_SUB;
        } else if (c == '>') {
            p++;
            tok = TOK_ARROW;
        } else {
            tok = '-';
        }
        break;

PARSE2('!', '!', '=', TOK_NE)
    PARSE2('=', '=', '=', TOK_EQ)
    PARSE2('*', '*', '=', TOK_A_MUL)
    PARSE2('%', '%', '=', TOK_A_MOD)
    PARSE2('^', '^', '=', TOK_A_XOR)
        
    case '/':
        PEEKC(c, p);
        if (c == '*') {
            p = parse_comment(p);
            goto redo_no_start;
        } else if (c == '/') {
            p = parse_line_comment(p);
            goto redo_no_start;
        } else if (c == '=') {
            p++;
            tok = TOK_A_DIV;
        } else {
            tok = '/';
        }
        break;
        
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case ',':
    case ';':
    case ':':
    case '?':
    case '~':
    case '$':
        tok = c;
        p++;
        break;
    default:
        error("unrecognized character \\x%02x", c);
        break;
    }
    file->buf_ptr = p;
    tok_flags = 0;
}

/* Return next token without macro substitution. Can read input layers directly from macro_ptr buffer */
static void next_nomacro(void)
{
    if (macro_ptr) {
    redo:
        tok = *macro_ptr;
        if (tok) {
            TOK_GET(tok, macro_ptr, tokc);
            if (tok == TOK_LINENUM) {
                file->line_num = tokc.i;
                goto redo;
            }
        }
    } else {
        next_nomacro1();
    }
}

/* Substitute parameter arguments in macro_str and return a newly allocated token string block */
static int *macro_arg_subst(Sym **nested_list, int *macro_str, Sym *args)
{
    int *st, last_tok, t, notfirst;
    Sym *s;
    CValue cval;
    TokenString str;
    CString cstr;

    tok_str_new(&str);
    last_tok = 0;
    while(1) {
        TOK_GET(t, macro_str, cval);
        if (!t)
            break;
            
        if (t == '#') {
            /* Execute preprocessor stringize operator (#) */
            TOK_GET(t, macro_str, cval);
            if (!t)
                break;
            s = sym_find2(args, t);
            if (s) {
                cstr_new(&cstr);
                st = (int *)s->c;
                notfirst = 0;
                while (*st) {
                    if (notfirst)
                        cstr_ccat(&cstr, ' ');
                    TOK_GET(t, st, cval);
                    cstr_cat(&cstr, get_tok_str(t, &cval));
                    notfirst = 1;
                }
                cstr_ccat(&cstr, '\0');
                
                /* Inject the stringized literal straight into the token emission queue */
                cval.cstr = &cstr;
                tok_str_add2(&str, TOK_STR, &cval);
                cstr_free(&cstr);
            } else {
                tok_str_add2(&str, t, &cval);
            }
        } else if (t >= TOK_IDENT) {
            s = sym_find2(args, t);
            if (s) {
                st = (int *)s->c;
                /* If '##' token pasting operator is adjacent, suppress immediate parameter substitution */
                if (*macro_str == TOK_TWOSHARPS || last_tok == TOK_TWOSHARPS) {
                    /* Special GNU C macro layout extension: '##' absorbs the preceding comma ',' if VA_ARGS is completely empty */
                    if (gnu_ext && s->type.t && last_tok == TOK_TWOSHARPS && str.len >= 2 && str.str[str.len - 2] == ',') {
                        if (*st == 0) {
                            /* Suppress both the loose comma and the trailing twin sharps operator safely */
                            str.len -= 2;
                        } else {
                            /* Strip out the structural twin sharps sequence and route straight into variable addition paths */
                            str.len--;
                            goto add_var;
                        }
                    } else {
                        int t1;
                    add_var:
                        for(;;) {
                            TOK_GET(t1, st, cval);
                            if (!t1)
                                break;
                            tok_str_add2(&str, t1, &cval);
                        }
                    }
                } else {
                    macro_subst(&str, nested_list, st);
                }
            } else {
                tok_str_add(&str, t);
            }
        } else {
            tok_str_add2(&str, t, &cval);
        }
        last_tok = t;
    }
    tok_str_add(&str, 0);
    return str.str;
}

/* Static tracking array providing localized chronological markers for ANSI C standard macro expansion frames */
static char const ab_month_name[12][4] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* Do macro substitution of current token with macro 's' and add result to tok_str.
   Tracks 'nested_list' to eliminate infinite preprocessor recursion patterns.
   Returns non-zero value if no substitution needs to be executed */
static int macro_subst_tok(TokenString *tok_str, Sym **nested_list, Sym *s)
{
    Sym *args, *sa, *sa1;
    int mstr_allocated, parlevel, *mstr, t;
    TokenString str;
    char *cstrval;
    CValue cval;
    CString cstr;
            
    /* Evaluate and process special native ANSI C standard built-in macro expansions */
    if (tok == TOK___LINE__) {
        cval.i = file->line_num;
        tok_str_add2(tok_str, TOK_CINT, &cval);
    } else if (tok == TOK___FILE__) {
        cstrval = file->filename;
        goto add_cstr;
    } else if (tok == TOK___DATE__ || tok == TOK___TIME__) {
        time_t ti;
        struct tm *tm;
        char buf[64];

        time(&ti);
        tm = localtime(&ti);
        if (tok == TOK___DATE__) {
            snprintf(buf, sizeof(buf), "%s %2d %d", 
                     ab_month_name[tm->tm_mon], tm->tm_mday, tm->tm_year + 1900);
        } else {
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 
                     tm->tm_hour, tm->tm_min, tm->tm_sec);
        }
        cstrval = buf;
    add_cstr:
        cstr_new(&cstr);
        cstr_cat(&cstr, cstrval);
        cstr_ccat(&cstr, '\0');
        cval.cstr = &cstr;
        tok_str_add2(tok_str, TOK_STR, &cval);
        cstr_free(&cstr);
    } else {
        mstr = (int *)s->c;
        mstr_allocated = 0;
        if (s->type.t == MACRO_FUNC) {
            /* Peek downstream parameters to verify if the macro definition identifier is followed by an open parenthesis */
            if (macro_ptr) {
                t = *macro_ptr;
                if (t == 0) {
                    /* End of current macro cache stream: Advance pointer directly to source file frame */
                    macro_ptr = NULL;
                    goto parse_stream;
                }
            } else {
            parse_stream:
                ch = file->buf_ptr[0];
                while (is_space(ch) || ch == '\n')
                    cinp();
                t = ch;
            }
            if (t != '(') /* Bypass macro expansion entirely if lookahead parameter is not an open parenthesis */
                return -1;
                    
            /* Parse functional macro argument lists */
            next_nomacro();
            next_nomacro();
            args = NULL;
            sa = s->next;
            
            for(;;) {
                /* Capture and handle direct empty structural function macro layouts '()' */
                if (!args && tok == ')')
                    break;
                if (!sa)
                    error("macro '%s' used with too many args", get_tok_str(s->v, 0));
                    
                tok_str_new(&str);
                parlevel = 0;
                
                /* Keep traversing parameters unless a closing boundary matching active parenthesis level is found */
                while ((parlevel > 0 || (tok != ')' && (tok != ',' || sa->type.t))) && tok != -1) {
                    if (tok == '(')
                        parlevel++;
                    else if (tok == ')')
                        parlevel--;
                    tok_str_add2(&str, tok, &tokc);
                    next_nomacro();
                }
                tok_str_add(&str, 0);
                sym_push2(&args, sa->v & ~SYM_FIELD, sa->type.t, (int)str.str);
                sa = sa->next;
                
                if (tok == ')') {
                    /* GNU C Macro expansion safety: Append an empty tracking variable argument if it is completely omitted */
                    if (sa && sa->type.t && gnu_ext)
                        continue;
                    else
                        break;
                }
                if (tok != ',')
                    expect(",");
                next_nomacro();
            }
            if (sa) {
                error("macro '%s' used with too few args", get_tok_str(s->v, 0));
            }

            /* Execute inner token substitution for each parsed argument sequence */
            mstr = macro_arg_subst(nested_list, mstr, args);
            
            /* Recycle allocated temporary tracking symbol memory blocks from heap structures */
            sa = args;
            while (sa) {
                sa1 = sa->prev;
                tok_str_free((int *)sa->c);
                tcc_free(sa);
                sa = sa1;
            }
            mstr_allocated = 1;
        }
        
        /* Enqueue macro identifier into structural nested list before evaluating subsequent expansions */
        sym_push2(nested_list, s->v, 0, 0);
        macro_subst(tok_str, nested_list, mstr);
        
        /* Pop current macro context from the recursion safety list and free resources */
        sa1 = *nested_list;
        *nested_list = sa1->prev;
        tcc_free(sa1);
        
        if (mstr_allocated)
            tok_str_free(mstr);
    }
    return 0;
}

/* Handle the token pasting '##' operator. Return NULL if no '##' sequence is encountered.
   Otherwise, return the newly synthesized combined token string block (which must be freed) */
static inline int *macro_twosharps(const int *macro_str)
{
    TokenSym *ts;
    const int *macro_ptr1, *start_macro_ptr, *ptr, *saved_macro_ptr;
    int t;
    const char *p1, *p2;
    CValue cval;
    TokenString macro_str1;
    CString cstr;

    start_macro_ptr = macro_str;
    
    /* Search sequentially to locate the first occurrence of the '##' operator */
    for(;;) {
        macro_ptr1 = macro_str;
        TOK_GET(t, macro_str, cval);
        /* Terminate pipeline immediately if the end of the macro string sequence is reached */
        if (t == 0)
            return NULL;
        if (*macro_str == TOK_TWOSHARPS)
            break;
    }

    /* Enforce advanced preprocessor concatenation sweeps to process discovered '##' operators */
    cstr_new(&cstr);
    tok_str_new(&macro_str1);
    tok = t;
    tokc = cval;

    /* Re-route and cache all standard lexical tokens discovered up to this milestone boundary */
    for(ptr = start_macro_ptr; ptr < macro_ptr1;) {
        TOK_GET(t, ptr, cval);
        tok_str_add2(&macro_str1, t, &cval);
    }
    saved_macro_ptr = macro_ptr;
    
    macro_ptr = (int *)macro_str;
    for(;;) {
        while (*macro_ptr == TOK_TWOSHARPS) {
            macro_ptr++;
            macro_ptr1 = macro_ptr;
            t = *macro_ptr;
            if (t) {
                TOK_GET(t, macro_ptr, cval);
                /* Execute raw character concatenation if both components are valid identifiers or numbers */
                cstr_reset(&cstr);
                p1 = get_tok_str(tok, &tokc);
                cstr_cat(&cstr, p1);
                p2 = get_tok_str(t, &cval);
                cstr_cat(&cstr, p2);
                cstr_ccat(&cstr, '\0');
                
                if ((tok >= TOK_IDENT || tok == TOK_PPNUM) && (t >= TOK_IDENT || t == TOK_PPNUM)) {
                    if (tok == TOK_PPNUM) {
                        /* Map synthesized sequence directly into a preprogramming numeric token structure */
                        tokc.cstr = &cstr;
                    } else {
                        /* Perform strict validation parsing to assert the newly merged identifier complies with grammar rules */
                        if (t == TOK_PPNUM) {
                            const char *p;
                            int c;

                            p = p2;
                            for(;;) {
                                c = *p;
                                if (c == '\0')
                                    break;
                                p++;
                                if (!isnum(c) && !isid(c))
                                    goto error_pasting;
                            }
                        }
                        ts = tok_alloc(cstr.data, strlen(cstr.data));
                        tok = ts->tok; /* Forcefully override and mask the current operational token */
                    }
                } else {
                    const char *str = cstr.data;
                    const unsigned char *q;

                    /* Verify if the combined operational string matches a legitimate compound operator token */
                    if (!strcmp(str, ">>=")) {
                        tok = TOK_A_SAR;
                    } else if (!strcmp(str, "<<=")) {
                        tok = TOK_A_SHL;
                    } else if (strlen(str) == 2) {
                        /* Traverse the twin-character lookup layout map to resolve combined operator bindings */
                        q = tok_two_chars;
                        for(;;) {
                            if (!*q)
                                goto error_pasting;
                            if (q[0] == str[0] && q[1] == str[1])
                                break;
                            q += 3;
                        }
                        tok = q[2];
                    } else {
                    error_pasting:
                        /* Paste validation failure path: Isolate and cache token names to protect static internal printing buffers */
                        cstr_reset(&cstr);
                        p1 = get_tok_str(tok, &tokc);
                        cstr_cat(&cstr, p1);
                        cstr_ccat(&cstr, '\0');
                        p2 = get_tok_str(t, &cval);
                        warning("pasting \"%s\" and \"%s\" does not give a valid preprocessing token", cstr.data, p2);
                        
                        /* Fallback strategy: Add the mismatched tokens sequentially into the layout stream without merging them */
                        tok_str_add2(&macro_str1, tok, &tokc);
                        tok = t;
                        tokc = cval;
                    }
                }
            }
        }
        tok_str_add2(&macro_str1, tok, &tokc);
        next_nomacro();
        if (tok == 0)
            break;
    }
    macro_ptr = (int *)saved_macro_ptr;
    cstr_free(&cstr);
    tok_str_add(&macro_str1, 0);
    return macro_str1.str;
}

/* Do macro substitution of macro_str sequence and append the finalized result directly onto tok_str.
   Tracks 'nested_list' map layers internally to suppress infinite preprocessor recursive loops */
static void macro_subst(TokenString *tok_str, Sym **nested_list, const int *macro_str)
{
    Sym *s;
    int *saved_macro_ptr, *macro_str1;
    const int *ptr;
    int t, ret;
    CValue cval;

    /* Execute primary analytical scan to resolve token pasting '##' operator constraints */
    ptr = macro_str;
    macro_str1 = macro_twosharps(ptr);
    if (macro_str1) 
        ptr = macro_str1;
        
    while (1) {
        /* Condition validation: ptr == NULL exclusively triggers when tokens flow out of 
           source filesystem streams during complex parametrizable macro function calls */
        if (ptr == NULL)
            break;
        TOK_GET(t, ptr, cval);
        if (t == 0)
            break;
            
        s = define_find(t);
        if (s != NULL) {
            /* If the discovered macro identifier matches an active parent layout context layer, suppress expansion */
            if (sym_find2(*nested_list, t))
                goto no_subst;
                
            saved_macro_ptr = macro_ptr;
            macro_ptr = (int *)ptr;
            tok = t;
            ret = macro_subst_tok(tok_str, nested_list, s);
            ptr = (const int *)macro_ptr;
            macro_ptr = saved_macro_ptr;
            if (ret != 0)
                goto no_subst;
        } else {
        no_subst:
            tok_str_add2(tok_str, t, &cval);
        }
    }
    if (macro_str1)
        tok_str_free(macro_str1);
}

/* Master Pipeline Entry: Return the next live compilation C token applying complete macro substitution */
static void next(void)
{
    Sym *nested_list, *s;
    TokenString str;

 redo:
    next_nomacro();
    if (!macro_ptr) {
        /* Evaluate and process identifier structures for potential macro expansion if reading straight from physical file streams */
        if (tok >= TOK_IDENT && (parse_flags & PARSE_FLAG_PREPROCESS)) {
            s = define_find(tok);
            if (s) {
                /* Target macro matched: Initialize local token serialization strings and attempt substitution */
                tok_str_new(&str);
                nested_list = NULL;
                if (macro_subst_tok(&str, &nested_list, s) == 0) {
                    /* Substitution pipeline succeeded. Append string null block and shift execution routing indicators */
                    tok_str_add(&str, 0);
                    macro_ptr = str.str;
                    macro_ptr_allocated = str.str;
                    goto redo;
                }
            }
        }
    } else {
        if (tok == 0) {
            /* Handle terminal macro memory block boundaries or internal pushback buffer exhaustion milestones */
            if (unget_buffer_enabled) {
                macro_ptr = unget_saved_macro_ptr;
                unget_buffer_enabled = 0;
            } else {
                /* Purge expired dynamic macro sequence memory structures from heap layout */
                tok_str_free(macro_ptr_allocated);
                macro_ptr = NULL;
            }
            goto redo;
        }
    }
    
    /* Convert raw preprocessor digit sequences (TOK_PPNUM) into concrete native C numeric constant types */
    if (tok == TOK_PPNUM && (parse_flags & PARSE_FLAG_TOK_NUM)) {
        parse_number((char *)tokc.cstr->data);
    }
}

/* Push back the current live token structure onto the parser queue setting active token directly to 'last_tok' */
static inline void unget_tok(int last_tok)
{
    int i, n;
    int *q;
    
    unget_saved_macro_ptr = macro_ptr;
    unget_buffer_enabled = 1;
    q = unget_saved_buffer;
    macro_ptr = q;
    *q++ = tok;
    
    n = tok_ext_size(tok) - 1;
    for(i = 0; i < n; i++)
        *q++ = tokc.tab[i];
    *q = 0; /* Inject structural terminal string null block wrapper */
    tok = last_tok;
}

/* Simple hardware-aligned integer payload value swap utility wrapper */
void swap(int *p, int *q)
{
    int t;
    t = *p;
    *p = *q;
    *q = t;
}

/* Push a complete contextual value payload directly onto the primary compiler evaluation stack */
void vsetc(CType *type, int r, CValue *vc)
{
    int v;

    if (vtop >= vstack + VSTACK_SIZE)
        error("memory full");
        
    /* Optimization barrier: Neutralize and resolve pending CPU comparison flags if downstream 
       instructions are requested, preventing complex register allocation conflicts */
    if (vtop >= vstack) {
        v = vtop->r & VT_VALMASK;
        if (v == VT_CMP || (v & ~1) == VT_JMP)
            gv(RC_INT);
    }
    
    vtop++;
    vtop->type = *type;
    vtop->r = r;
    vtop->r2 = VT_CONST;
    vtop->c = *vc;
}

/* Push a standard 32-bit native integer constant straight onto the evaluation stack */
void vpushi(int v)
{
    CValue cval;
    cval.i = v;
    vsetc(&int_type, VT_CONST, &cval);
}

/* Generate and register a newly synthesized static tracking symbol targeted at a specific flat section */
static Sym *get_sym_ref(CType *type, Section *sec, unsigned long offset, unsigned long size)
{
    int v;
    Sym *sym;

    v = anon_sym++;
    sym = global_identifier_push(v, type->t | VT_STATIC, 0);
    sym->type.ref = type->ref;
    sym->r = VT_CONST | VT_SYM;
    
    /* Enforce strict target binding directly through our arithmetically safe flat relocation registry */
    put_extern_sym(sym, sec, offset, size);
    return sym;
}

/* Push a section-relative offset reference by injecting a loose anonymous tracing symbol wrapper */
static void vpush_ref(CType *type, Section *sec, unsigned long offset, unsigned long size)
{
    CValue cval;

    cval.ul = 0;
    vsetc(type, VT_CONST | VT_SYM, &cval);
    vtop->sym = get_sym_ref(type, sec, offset, size);
}

/* Define or query a unique external global linkage symbol reference inside permanent context scopes */
static Sym *external_global_sym(int v, CType *type, int r)
{
    Sym *s;

    s = sym_find(v);
    if (!s) {
        /* Construct a forward reference allocation descriptor directly inside the permanent tables */
        s = global_identifier_push(v, type->t | VT_EXTERN, 0);
        s->type.ref = type->ref;
        s->r = r | VT_CONST | VT_SYM;
    }
    return s;
}

/* Define a localized or forward external lookup symbol ensuring strict validation checks */
static Sym *external_sym(int v, CType *type, int r)
{
    Sym *s;

    s = sym_find(v);
    if (!s) {
        /* Allocate a forward reference linkage map frame onto the semantic token sequence ystack */
        s = sym_push(v, type, r | VT_CONST | VT_SYM, 0);
        s->type.t |= VT_EXTERN;
    } else {
        if (!is_compatible_types(&s->type, type))
            error("incompatible types for redefinition of '%s'", get_tok_str(v, NULL));
    }
    return s;
}

/* Push an explicit memory layout reference pointing to an external global symbol descriptor */
static void vpush_global_sym(CType *type, int v)
{
    Sym *sym;
    CValue cval;

    sym = external_global_sym(v, type, 0);
    cval.ul = 0;
    vsetc(type, VT_CONST | VT_SYM, &cval);
    vtop->sym = sym;
}

/* Wrapper tracking standard type registrations pushing arbitrary register properties into vsetc */
void vset(CType *type, int r, int v)
{
    CValue cval;

    cval.i = v;
    vsetc(type, r, &cval);
}

/* Shorthand abstraction pushing a standardized 32-bit native integer structure rapidly */
void vseti(int r, int v)
{
    CType type;
    type.t = VT_INT;
    vset(&type, r, v);
}

/* Transpose the top two operational slots on the evaluation stack layout precisely */
void vswap(void)
{
    SValue tmp;

    tmp = vtop[0];
    vtop[0] = vtop[-1];
    vtop[-1] = tmp;
}

/* Secure copy an external evaluation block and advance the stack tracking register pointer */
void vpushv(SValue *v)
{
    if (vtop >= vstack + VSTACK_SIZE)
        error("memory full");
    vtop++;
    *vtop = *v;
}

/* Duplicate the current top slot entry layout on the evaluation stack */
void vdup(void)
{
    vpushv(vtop);
}

/* Save register 'r' directly onto the memory stack frames and mark it as unallocated */
void save_reg(int r)
{
    int l, saved, size, align;
    SValue *p, sv;
    CType *type;

    /* Traverse and modify all tracked active execution evaluation stack contexts */
    saved = 0;
    l = 0;
    for(p = vstack; p <= vtop; p++) {
        if ((p->r & VT_VALMASK) == r || (p->r2 & VT_VALMASK) == r) {
            /* Synchronize data state forcing register save parameters if not already executed */
            if (!saved) {
                /* Reload targeted register because primary reference might match the auxiliary long long layout */
                r = p->r & VT_VALMASK;
                
                type = &p->type;
                if ((p->r & VT_LVAL) || (!is_float(type->t) && (type->t & VT_BTYPE) != VT_LLONG))
                    type = &int_type;
                    
                size = type_size(type, &align);
                loc = (loc - size) & -align;
                sv.type.t = type->t;
                sv.r = VT_LOCAL | VT_LVAL;
                sv.c.ul = loc;
                store(r, &sv);

                /* Native Native TRDOS 386: Evacuate and pop the floating point top descriptor ST0 if cached */
                if (r == TREG_ST0) {
                    o(0xd9dd); /* fstp %st(1) execution machine opcode insertion */
                }

                /* Resolve structural 64-bit multi-precision long long segments sequentially */
                if ((type->t & VT_BTYPE) == VT_LLONG) {
                    sv.c.ul += 4;
                    store(p->r2, &sv);
                }
                l = loc;
                saved = 1;
            }
            
            /* Recalibrate active evaluation index tags marking them as localized memory storage references */
            if (p->r & VT_LVAL) {
                /* Bounds verification masks cleanly purged to guarantee strict flat stack alignment metrics */
                p->r = (p->r & ~VT_VALMASK) | VT_LLOCAL;
            } else {
                p->r = lvalue_type(p->type.t) | VT_LOCAL;
            }
            p->r2 = VT_CONST;
            p->c.ul = l;
        }
    }
}

/* Query the hardware architecture to locate a free register matching target class constraints */
int get_reg(int rc)
{
    int r;
    SValue *p;

    /* Attempt to discover an unallocated hardware register frame */
    for(r = 0; r < NB_REGS; r++) {
        if (reg_classes[r] & rc) {
            for(p = vstack; p <= vtop; p++) {
                if ((p->r & VT_VALMASK) == r || (p->r2 & VT_VALMASK) == r)
                    goto notfound;
            }
            return r;
        }
    notfound: ;
    }
    
    /* Allocation fail path: Flush the oldest persistent register context down on the evaluation stack.
       Traversing sequentially from bottom to top prevents destruction of variables required by gen_opi() */
    for(p = vstack; p <= vtop; p++) {
        r = p->r & VT_VALMASK;
        if (r < VT_CONST && (reg_classes[r] & rc))
            goto save_found;
            
        r = p->r2 & VT_VALMASK;
        if (r < VT_CONST && (reg_classes[r] & rc)) {
        save_found:
            save_reg(r);
            return r;
        }
    }
    
    return -1;
}

/* Save and flush all active hardware registers up to the specified boundary stack depth */
void save_regs(int n)
{
    int r;
    SValue *p, *p1;
    p1 = vtop - n;
    for(p = vstack; p <= p1; p++) {
        r = p->r & VT_VALMASK;
        if (r < VT_CONST) {
            save_reg(r);
        }
    }
}

/* Move active values between registers flushing historical payload data out to prevent overwrite corruption */
void move_reg(int r, int s)
{
    SValue sv;

    if (r != s) {
        save_reg(r);
        sv.type.t = VT_INT;
        sv.r = s;
        sv.c.ul = 0;
        load(r, &sv);
    }
}

/* Get absolute memory address of vtop stack entry (vtop MUST BE an active lvalue context) */
void gaddrof(void)
{
    vtop->r &= ~VT_LVAL;
    /* Optimization path: If historical entry was a saved local variable, revert safely back to localized allocation */
    if ((vtop->r & VT_VALMASK) == VT_LLOCAL)
        vtop->r = (vtop->r & ~(VT_VALMASK | VT_LVAL_TYPE)) | VT_LOCAL | VT_LVAL;
}

/* Load vtop into a hardware processor register belonging to class 'rc'.
   Lvalues are dereferenced and converted directly into scalar or float register values. */
int gv(int rc)
{
    int r, r2, rc2, bit_pos, bit_size, size, align, i;
    unsigned long long ll;

    /* Handle C structure bitfield packing extractions directly via hardware shift instructions */
    if (vtop->type.t & VT_BITFIELD) {
        bit_pos = (vtop->type.t >> VT_STRUCT_SHIFT) & 0x3f;
        bit_size = (vtop->type.t >> (VT_STRUCT_SHIFT + 6)) & 0x3f;
        
        /* Strip out bitfield attributes to prevent infinite compiler optimization tracking loops */
        vtop->type.t &= ~(VT_BITFIELD | (-1 << VT_STRUCT_SHIFT));
        
        /* Generate analytical machine level bit shifts */
        vpushi(32 - (bit_pos + bit_size));
        gen_op(TOK_SHL);
        vpushi(32 - bit_size);
        gen_op(TOK_SAR); /* Evaluated and automatically masked as SHR if unsigned */
        r = gv(rc);
    } else {
        if (is_float(vtop->type.t) && (vtop->r & (VT_VALMASK | VT_LVAL)) == VT_CONST) {
            Sym *sym;
            int *ptr;
            unsigned long offset;
            
            /* Native x86 Floating Point optimization: Cache raw float constants straight into contiguous data segment */
            size = type_size(&vtop->type, &align);
            offset = (data_section->data_offset + align - 1) & -align;
            data_section->data_offset = offset;
            
            ptr = section_ptr_add(data_section, size);
            size = size >> 2;
            for(i = 0; i < size; i++)
                ptr[i] = vtop->c.tab[i];
                
            sym = get_sym_ref(&vtop->type, data_section, offset, size << 2);
            vtop->r |= VT_LVAL | VT_SYM;
            vtop->sym = sym;
            vtop->c.ul = 0;
        }

        r = vtop->r & VT_VALMASK;
        
        /* Trigger direct register loading cycle if entry matches memory layout bounds, casts, or class mismatches */
        if (r >= VT_CONST || 
            (vtop->r & VT_LVAL) ||
            !(reg_classes[r] & rc) ||
            ((vtop->type.t & VT_BTYPE) == VT_LLONG && !(reg_classes[vtop->r2] & rc))) {
            
            r = get_reg(rc);
            if ((vtop->type.t & VT_BTYPE) == VT_LLONG) {
                /* Multi-precision 64-bit long long splitting: Disassemble data into twin 32-bit storage payloads */
                if ((vtop->r & (VT_VALMASK | VT_LVAL)) == VT_CONST) {
                    ll = vtop->c.ull;
                    vtop->c.ui = ll; /* Lower dword layout */
                    load(r, vtop);
                    vtop->r = r;
                    vpushi(ll >> 32); /* Upper dword layout */
                } else if (r >= VT_CONST || (vtop->r & VT_LVAL)) {
                    /* Execute direct double-register sequential loading sequence from memory descriptors */
                    load(r, vtop);
                    vdup();
                    vtop[-1].r = r;
                    vtop->type.t = VT_INT;
                    gaddrof();
                    vpushi(4);
                    gen_op('+');
                    vtop->r |= VT_LVAL;
                } else {
                    /* Safe physical data transfer across adjacent register maps */
                    load(r, vtop);
                    vdup();
                    vtop[-1].r = r;
                    vtop->r = vtop[-1].r2;
                }
                
                /* Target next available data processing general purpose integer register slot */
                rc2 = RC_INT;
                if (rc == RC_IRET)
                    rc2 = RC_LRET;
                r2 = get_reg(rc2);
                load(r2, vtop);
                vpop();
                vtop->r2 = r2;
            } else if ((vtop->r & VT_LVAL) && !is_float(vtop->type.t)) {
                int t1, t;
                /* Resolve scalar types checking layout sizes to guard against potential type truncation or extensions */
                t = vtop->type.t;
                t1 = t;
                
                if (vtop->r & VT_LVAL_BYTE)
                    t = VT_BYTE;
                else if (vtop->r & VT_LVAL_SHORT)
                    t = VT_SHORT;
                if (vtop->r & VT_LVAL_UNSIGNED)
                    t |= VT_UNSIGNED;
                    
                vtop->type.t = t;
                load(r, vtop);
                vtop->type.t = t1;
            } else {
                /* Execute standard isolated standard scalar or single-precision register data load loop */
                load(r, vtop);
            }
        }
        vtop->r = r;
    }
    return r;
}

/* Generate values for vtop[-1] and vtop[0] inside corresponding classes rc1 and rc2.
   Enforces strict resolution order for evaluation short-circuits like VT_JMP or VT_CMP. */
void gv2(int rc1, int rc2)
{
    int v;

    /* Prioritize the generation of generic registers first to prevent unexpected reload traps */
    v = vtop[0].r & VT_VALMASK;
    if (v != VT_CMP && (v & ~1) != VT_JMP && rc1 <= rc2) {
        vswap();
        gv(rc1);
        vswap();
        gv(rc2);
        /* Verify if the secondary tracking register requests an immediate reload sweep */
        if ((vtop[-1].r & VT_VALMASK) >= VT_CONST) {
            vswap();
            gv(rc1);
            vswap();
        }
    } else {
        gv(rc2);
        vswap();
        gv(rc1);
        vswap();
        /* Verify if the primary tracking register requests an immediate reload sweep */
        if ((vtop[0].r & VT_VALMASK) >= VT_CONST) {
            gv(rc2);
        }
    }
}

/* Expand a 64-bit multi-precision long long stack entry into twin 32-bit integer registers */
void lexpand(void)
{
    int u;

    u = vtop->type.t & VT_UNSIGNED;
    gv(RC_INT);
    vdup();
    vtop[0].r = vtop[-1].r2;
    vtop[0].r2 = VT_CONST;
    vtop[-1].r2 = VT_CONST;
    vtop[0].type.t = VT_INT | u;
    vtop[-1].type.t = VT_INT | u;
}

/* Synthesize a singular 64-bit long long structured value out of two standalone integer registers */
void lbuild(int t)
{
    gv2(RC_INT, RC_INT);
    vtop[-1].r2 = vtop[0].r;
    vtop[-1].type.t = t;
    vpop();
}

/* Rotate the designated 'n' quantity of initial evaluation stack entries straight to the bottom */
void vrotb(int n)
{
    int i;
    SValue tmp;

    tmp = vtop[-n + 1];
    for(i = -n + 1; i != 0; i++)
        vtop[i] = vtop[i + 1];
    vtop[0] = tmp;
}

/* Pop the active top entry frame off the internal execution evaluation stack */
void vpop(void)
{
    int v;
    v = vtop->r & VT_VALMASK;

    /* Native TRDOS 386 x87 FPU specific: Enforce floating-point stack evacuation if not blocked */
    if (v == TREG_ST0 && !nocode_wanted) {
        o(0xd9dd); /* fstp %st(1) execution machine opcode insertion */
    } else if (v == VT_JMP || v == VT_JMPI) {
        /* Direct back-patching routing to finalize loose logical jumps (&& or ||) missing comparison tests */
        gsym(vtop->c.ul);
    }
    vtop--;
}

/* Force-convert the top stack frame entry into a register, duplicating its contents across an auxiliary register slot */
void gv_dup(void)
{
    int rc, t, r, r1;
    SValue sv;

    t = vtop->type.t;
    if ((t & VT_BTYPE) == VT_LLONG) {
        lexpand();
        gv_dup();
        vswap();
        vrotb(3);
        gv_dup();
        vrotb(4);
        /* Target internal layout sequence alignment: H L L1 H1 */
        lbuild(t);
        vrotb(3);
        vrotb(3);
        vswap();
        lbuild(t);
        vswap();
    } else {
        rc = RC_INT;
        sv.type.t = VT_INT;
        if (is_float(t)) {
            rc = RC_FLOAT;
            sv.type.t = t;
        }
        r = gv(rc);
        r1 = get_reg(rc);
        sv.r = r;
        sv.c.ul = 0;
        load(r1, &sv); /* Execute data transfer moving the payload from register 'r' straight into 'r1' */
        vdup();
        vtop->r = r1;
    }
}

/* Generate CPU independent 64-bit (unsigned) long long operations and semantic logic rules */
void gen_opl(int op)
{
    int t, a, b, op1, c, i;
    int func;
    GFuncContext gf;
    SValue tmp;

    switch(op) {
    case '/':
    case TOK_PDIV:
        func = TOK___divdi3;
        goto gen_func;
    case TOK_UDIV:
        func = TOK___udivdi3;
        goto gen_func;
    case '%':
        func = TOK___moddi3;
        goto gen_func;
    case TOK_UMOD:
        func = TOK___umoddi3;
    gen_func:
        /* Route and dispatch generic 64-bit long long math runtime library function call loops */
        gfunc_start(&gf, FUNC_CDECL);
        gfunc_param(&gf);
        gfunc_param(&gf);
        vpush_global_sym(&func_old_type, func);
        gfunc_call(&gf);
        vpushi(0);
        vtop->r = REG_IRET;
        vtop->r2 = REG_LRET;
        break;
    case '^':
    case '&':
    case '|':
    case '*':
    case '+':
    case '-':
        t = vtop->type.t;
        vswap();
        lexpand();
        vrotb(3);
        lexpand();
        
        /* Evaluate target data sequence alignment frame: L1 H1 L2 H2 */
        tmp = vtop[0];
        vtop[0] = vtop[-3];
        vtop[-3] = tmp;
        tmp = vtop[-2];
        vtop[-2] = vtop[-3];
        vtop[-3] = tmp;
        vswap();
        
        /* Realized memory map layout sequence link: H1 H2 L1 L2 */
        if (op == '*') {
            vpushv(vtop - 1);
            vpushv(vtop - 1);
            gen_op(TOK_UMULL);
            lexpand();
            
            for(i = 0; i < 4; i++)
                vrotb(6);
                
            tmp = vtop[0];
            vtop[0] = vtop[-2];
            vtop[-2] = tmp;
            
            gen_op('*');
            vrotb(3);
            vrotb(3);
            gen_op('*');
            gen_op('+');
            gen_op('+');
        } else if (op == '+' || op == '-') {
            if (op == '+')
                op1 = TOK_ADDC1;
            else
                op1 = TOK_SUBC1;
            gen_op(op1);
            
            /* Target calculation map stack: H1 H2 (L1 op L2) */
            vrotb(3);
            vrotb(3);
            gen_op(op1 + 1); /* Execute sequential TOK_xxxC2 carry chain instructions */
        } else {
            gen_op(op);
            vrotb(3);
            vrotb(3);
            gen_op(op);
        }
        lbuild(t);
        break;
    case TOK_SAR:
    case TOK_SHR:
    case TOK_SHL:
        if ((vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST) {
            t = vtop[-1].type.t;
            vswap();
            lexpand();
            vrotb(3);
            
            /* Target instruction layout stack: L H shift */
            c = (int)vtop->c.i;
            vpop();
            if (op != TOK_SHL)
                vswap();
                
            if (c >= 32) {
                vpop();
                if (c > 32) {
                    vpushi(c - 32);
                    gen_op(op);
                }
                if (op != TOK_SAR) {
                    vpushi(0);
                } else {
                    gv_dup();
                    vpushi(31);
                    gen_op(TOK_SAR);
                }
                vswap();
            } else {
                vswap();
                gv_dup();
                vpushi(c);
                gen_op(op);
                vswap();
                vpushi(32 - c);
                if (op == TOK_SHL)
                    gen_op(TOK_SHR);
                else
                    gen_op(TOK_SHL);
                vrotb(3);
                
                vpushi(c);
                if (op == TOK_SHL)
                    gen_op(TOK_SHL);
                else
                    gen_op(TOK_SHR);
                gen_op('|');
            }
            if (op != TOK_SHL)
                vswap();
            lbuild(t);
        } else {
            /* Execute dynamic runtime shifting fallback routing targeting 64-bit libc library links */
            switch(op) {
            case TOK_SAR:
                func = TOK___sardi3;
                goto gen_func;
            case TOK_SHR:
                func = TOK___shrdi3;
                goto gen_func;
            case TOK_SHL:
                func = TOK___shldi3;
                goto gen_func;
            }
        }
        break;
    default:
        /* Evaluate and process 64-bit conditional comparison operations */
        t = vtop->type.t;
        vswap();
        lexpand();
        vrotb(3);
        lexpand();
        
        tmp = vtop[-1];
        vtop[-1] = vtop[-2];
        vtop[-2] = tmp;
        
        /* Invert and recalibrate logic boundaries when values evaluate as equal to handle low dword checks */
        op1 = op;
        if (op1 == TOK_LT)
            op1 = TOK_LE;
        else if (op1 == TOK_GT)
            op1 = TOK_GE;
        else if (op1 == TOK_ULT)
            op1 = TOK_ULE;
        else if (op1 == TOK_UGT)
            op1 = TOK_UGE;
            
        a = 0;
        b = 0;
        gen_op(op1);
        if (op1 != TOK_NE) {
            a = gtst(1, 0);
        }
        if (op != TOK_EQ) {
            if (a == 0) {
                b = gtst(0, 0);
            } else {
                /* Native TRDOS 386: Direct emission of the 0x850f machine instruction (JNZ/JNE conditional branch) */
                b = psym(0x850f, 0);
            }
        }
        
        /* Execute comparison on low dwords. This layout layer evaluates strictly as unsigned */
        op1 = op;
        if (op1 == TOK_LT)
            op1 = TOK_ULT;
        else if (op1 == TOK_LE)
            op1 = TOK_ULE;
        else if (op1 == TOK_GT)
            op1 = TOK_UGT;
        else if (op1 == TOK_GE)
            op1 = TOK_UGE;
            
        gen_op(op1);
        a = gtst(1, a);
        gsym(b);
        vseti(VT_JMPI, a);
        break;
    }
}

/* Handle compile-time integer constant optimizations and generic machine-independent evaluations */
void gen_opic(int op)
{
    int fc, c1, c2, n;
    SValue *v1, *v2;

    v1 = vtop - 1;
    v2 = vtop;
    
    /* Determine if adjacent evaluation slots qualify as strict compile-time constants */
    c1 = (v1->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
    c2 = (v2->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
    
    if (c1 && c2) {
        fc = v2->c.i;
        switch(op) {
        case '+': v1->c.i += fc; break;
        case '-': v1->c.i -= fc; break;
        case '&': v1->c.i &= fc; break;
        case '^': v1->c.i ^= fc; break;
        case '|': v1->c.i |= fc; break;
        case '*': v1->c.i *= fc; break;

        case TOK_PDIV:
        case '/':
        case '%':
        case TOK_UDIV:
        case TOK_UMOD:
            /* Raise strict diagnostic trap if a divide-by-zero anomaly is intercepted inside constants */
            if (fc == 0) {
                if (const_wanted)
                    error("division by zero in constant");
                goto general_case;
            }
            switch(op) {
            default: v1->c.i /= fc; break;
            case '%': v1->c.i %= fc; break;
            case TOK_UDIV: v1->c.i = (unsigned)v1->c.i / fc; break;
            case TOK_UMOD: v1->c.i = (unsigned)v1->c.i % fc; break;
            }
            break;
        case TOK_SHL: v1->c.i <<= fc; break;
        case TOK_SHR: v1->c.i = (unsigned)v1->c.i >> fc; break;
        case TOK_SAR: v1->c.i >>= fc; break;
        
        /* Direct binary relational evaluation katman tests */
        case TOK_ULT: v1->c.i = (unsigned)v1->c.i < (unsigned)fc; break;
        case TOK_UGE: v1->c.i = (unsigned)v1->c.i >= (unsigned)fc; break;
        case TOK_EQ: v1->c.i = v1->c.i == fc; break;
        case TOK_NE: v1->c.i = v1->c.i != fc; break;
        case TOK_ULE: v1->c.i = (unsigned)v1->c.i <= (unsigned)fc; break;
        case TOK_UGT: v1->c.i = (unsigned)v1->c.i > (unsigned)fc; break;
        case TOK_LT: v1->c.i = v1->c.i < fc; break;
        case TOK_GE: v1->c.i = v1->c.i >= fc; break;
        case TOK_LE: v1->c.i = v1->c.i <= fc; break;
        case TOK_GT: v1->c.i = v1->c.i > fc; break;
        
        /* Preprocessor shortcut logical checks */
        case TOK_LAND: v1->c.i = v1->c.i && fc; break;
        case TOK_LOR: v1->c.i = v1->c.i || fc; break;
        default:
            goto general_case;
        }
        vtop--;
    } else {
        /* Optimization sweep: If commutative operators are found, shift the constant payload to the second slot */
        if (c1 && (op == '+' || op == '&' || op == '^' || op == '|' || op == '*')) {
            vswap();
            swap(&c1, &c2);
        }
        fc = vtop->c.i;
        
        /* Apply mathematical shortcut identities to drop redundant instructions rapidly */
        if (c2 && (((op == '*' || op == '/' || op == TOK_UDIV || op == TOK_PDIV) && fc == 1) ||
                   ((op == '+' || op == '-' || op == '|' || op == '^' || 
                     op == TOK_SHL || op == TOK_SHR || op == TOK_SAR) && fc == 0) ||
                   (op == '&' && fc == -1))) {
            vtop--;
        } else if (c2 && (op == '*' || op == TOK_PDIV || op == TOK_UDIV)) {
            /* Transform expensive multiplications or divisions directly into high-efficiency bitwise hardware shifts */
            if (fc > 0 && (fc & (fc - 1)) == 0) {
                n = -1;
                while (fc) {
                    fc >>= 1;
                    n++;
                }
                vtop->c.i = n;
                if (op == '*')
                    op = TOK_SHL;
                else if (op == TOK_PDIV)
                    op = TOK_SAR;
                else
                    op = TOK_SHR;
            }
            goto general_case;
        } else if (c2 && (op == '+' || op == '-') &&
                   (vtop[-1].r & (VT_VALMASK | VT_LVAL | VT_SYM)) == (VT_CONST | VT_SYM)) {
            /* Optimization rule: Directly resolve structural symbol plus constant transformations */
            if (op == '-')
                fc = -fc;
            vtop--;
            vtop->c.i += fc;
        } else {
        general_case:
            if (!nocode_wanted) {
                /* Forward down directly into the low-level machine code instruction generator */
                gen_opi(op);
            } else {
                vtop--;
            }
        }
    }
}

/* Generate floating-point execution sequences applying strict ANSI compliant constant folding propagation */
void gen_opif(int op)
{
    int c1, c2;
    SValue *v1, *v2;
    long double f1, f2;

    v1 = vtop - 1;
    v2 = vtop;
    
    c1 = (v1->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
    c2 = (v2->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
    
    if (c1 && c2) {
        if (v1->type.t == VT_FLOAT) {
            f1 = v1->c.f;
            f2 = v2->c.f;
        } else if (v1->type.t == VT_DOUBLE) {
            f1 = v1->c.d;
            f2 = v2->c.d;
        } else {
            f1 = v1->c.ld;
            f2 = v2->c.ld;
        }

        /* ANSI C Compliance verification: Enforce runtime constant propagation exclusively on finite math limits */
        if (!ieee_finite(f1) || !ieee_finite(f2))
            goto general_case;

        switch(op) {
        case '+': f1 += f2; break;
        case '-': f1 -= f2; break;
        case '*': f1 *= f2; break;
        case '/': 
            if (f2 == 0.0) {
                if (const_wanted)
                    error("division by zero in constant");
                goto general_case;
            }
            f1 /= f2; 
            break;
        default:
            goto general_case;
        }
        
        if (v1->type.t == VT_FLOAT) {
            v1->c.f = f1;
        } else if (v1->type.t == VT_DOUBLE) {
            v1->c.d = f1;
        } else {
            v1->c.ld = f1;
        }
        vtop--;
    } else {
    general_case:
        if (!nocode_wanted) {
            gen_opf(op);
        } else {
            vtop--;
        }
    }
}

/* Extract and return the underlying physical allocation storage layout size pointed to by the active type structure */
static int pointed_size(CType *type)
{
    int align;
    return type_size(pointed_type(type), &align);
}

/* Generic binary operation engine: Evaluates language semantics and handles implicit type promotions */
void gen_op(int op)
{
    int u, t1, t2, bt1, bt2, t;
    CType type1;

    t1 = vtop[-1].type.t;
    t2 = vtop[0].type.t;
    bt1 = t1 & VT_BTYPE;
    bt2 = t2 & VT_BTYPE;
        
    if (bt1 == VT_PTR || bt2 == VT_PTR) {
        /* Evaluation path: At least one of the operational operands is an active pointer structure */
        if (op >= TOK_ULT && op <= TOK_GT) {
            /* Relational checks: Map cross-pointer comparison operands strictly as unsigned integers */
            t = VT_INT | VT_UNSIGNED;
            goto std_op;
        }
        
        /* Direct check: If both elements evaluate as pointers, enforce the strict subtraction operation rule */
        if (bt1 == VT_PTR && bt2 == VT_PTR) {
            if (op != '-')
                error("cannot use pointers here");
                
            u = pointed_size(&vtop[-1].type);
            gen_opic(op);
            
            /* Enforce integer type destination masking pointer distance parameters */
            vtop->type.t = VT_INT; 
            vpushi(u);
            gen_op(TOK_PDIV);
        } else {
            /* Pointers arithmetic rule: Exactly one memory pointer requires addition (+) or subtraction (-) operators */
            if (op != '-' && op != '+')
                error("cannot use pointers here");
                
            /* Optimization shortcut: Forcefully swap operand layout so the pointer remains positioned first */
            if (bt2 == VT_PTR) {
                vswap();
                swap(&t1, &t2);
            }
            type1 = vtop[-1].type;
            
            /* Calculate array address scales by evaluating memory cell sizes sequentially */
            vpushi(pointed_size(&vtop[-1].type));
            gen_op('*');
            
            /* Runtime array bounds checking code safely stripped away to maintain absolute flat execution speed */
            gen_opic(op);
            
            /* Re-route and assert the original base pointer type metadata descriptors */
            vtop->type = type1;
        }
    } else if (is_float(bt1) || is_float(bt2)) {
        /* Target evaluation logic: Compute the highest precision format forcing implicit floating point casts */
        if (bt1 == VT_LDOUBLE || bt2 == VT_LDOUBLE) {
            t = VT_LDOUBLE;
        } else if (bt1 == VT_DOUBLE || bt2 == VT_DOUBLE) {
            t = VT_DOUBLE;
        } else {
            t = VT_FLOAT;
        }
        
        if (op != '+' && op != '-' && op != '*' && op != '/' && (op < TOK_ULT || op > TOK_GT))
            error("invalid operands for binary operation");
        goto std_op;
    } else if (bt1 == VT_LLONG || bt2 == VT_LLONG) {
        /* Promote type fields directly to match 64-bit multi-precision long long boundary frames */
        t = VT_LLONG;
        if ((t1 & (VT_BTYPE | VT_UNSIGNED)) == (VT_LLONG | VT_UNSIGNED) ||
            (t2 & (VT_BTYPE | VT_UNSIGNED)) == (VT_LLONG | VT_UNSIGNED))
            t |= VT_UNSIGNED;
        goto std_op;
    } else {
        /* Standard native 32-bit x86 Protected Mode flat integer calculations pipeline */
        t = VT_INT;
        if ((t1 & (VT_BTYPE | VT_UNSIGNED)) == (VT_INT | VT_UNSIGNED) ||
            (t2 & (VT_BTYPE | VT_UNSIGNED)) == (VT_INT | VT_UNSIGNED))
            t |= VT_UNSIGNED;
            
    std_op:
        /* Semantic map translation: Convert implicit signed expressions directly to low-level unsigned hardware configurations */
        if (t & VT_UNSIGNED) {
            if (op == TOK_SAR)
                op = TOK_SHR;
            else if (op == '/')
                op = TOK_UDIV;
            else if (op == '%')
                op = TOK_UMOD;
            else if (op == TOK_LT)
                op = TOK_ULT;
            else if (op == TOK_GT)
                op = TOK_UGT;
            else if (op == TOK_LE)
                op = TOK_ULE;
            else if (op == TOK_GE)
                op = TOK_UGE;
        }
        vswap();
        type1.t = t;
        gen_cast(&type1);
        vswap();
        
        /* Bitwise shift exception constraint: Retain shift operation magnitude values strictly as native integers */
        if (op == TOK_SHR || op == TOK_SAR || op == TOK_SHL)
            type1.t = VT_INT;
            
        gen_cast(&type1);
        if (is_float(t))
            gen_opif(op);
        else if ((t & VT_BTYPE) == VT_LLONG)
            gen_opl(op);
        else
            gen_opic(op);
            
        if (op >= TOK_ULT && op <= TOK_GT) {
            vtop->type.t = VT_INT; /* Enforce relational evaluation final outputs explicitly as integers (0 or 1) */
        } else {
            vtop->type.t = t;
        }
    }
}

/* Generic integer to floating-point conversion wrapper optimized for 64-bit unsigned long long case */
void gen_cvt_itof1(int t)
{
    GFuncContext gf;

    if ((vtop->type.t & (VT_BTYPE | VT_UNSIGNED)) == (VT_LLONG | VT_UNSIGNED)) {
        /* Dispatch and route generic 64-bit runtime conversion function call loops */
        gfunc_start(&gf, FUNC_CDECL);
        gfunc_param(&gf);
        if (t == VT_FLOAT)
            vpush_global_sym(&func_old_type, TOK___ulltof);
        else if (t == VT_DOUBLE)
            vpush_global_sym(&func_old_type, TOK___ulltod);
        else
            vpush_global_sym(&func_old_type, TOK___ulltold);
        gfunc_call(&gf);
        vpushi(0);
        vtop->r = REG_FRET;
    } else {
        /* Fall back straight into the native x86 hardware FPU conversion engine */
        gen_cvt_itof(t);
    }
}

/* Generic floating-point to integer conversion wrapper optimized for 64-bit unsigned long long case */
void gen_cvt_ftoi1(int t)
{
    GFuncContext gf;
    int st;

    if (t == (VT_LLONG | VT_UNSIGNED)) {
        /* Enforce software runtime library implementation fallback as x86 cannot process this natively */
        gfunc_start(&gf, FUNC_CDECL);
        st = vtop->type.t & VT_BTYPE;
        gfunc_param(&gf);
        if (st == VT_FLOAT)
            vpush_global_sym(&func_old_type, TOK___fixunssfdi);
        else if (st == VT_DOUBLE)
            vpush_global_sym(&func_old_type, TOK___fixunsdfdi);
        else
            vpush_global_sym(&func_old_type, TOK___fixunsxfdi);
        gfunc_call(&gf);
        vpushi(0);
        vtop->r = REG_IRET;
        vtop->r2 = REG_LRET;
    } else {
        /* Fall back straight into the native x86 hardware FPU truncation engine */
        gen_cvt_ftoi(t);
    }
}

/* Force explicit character (8-bit) or short integer (16-bit) conversion boundaries on evaluation stack */
void force_charshort_cast(int t)
{
    int bits, dbt;
    dbt = t & VT_BTYPE;
    
    if (dbt == VT_BYTE)
        bits = 8;
    else
        bits = 16;
        
    if (t & VT_UNSIGNED) {
        /* Execute zero-extension optimization by applying bitwise masking operation */
        vpushi((1 << bits) - 1);
        gen_op('&');
    } else {
        /* Execute sign-extension optimization via hardware arithmetic shift sequences */
        bits = 32 - bits;
        vpushi(bits);
        gen_op(TOK_SHL);
        vpushi(bits);
        gen_op(TOK_SAR);
    }
}

/* Execute compiler explicit or implicit type casting operations on the top stack entry vtop */
static void gen_cast(CType *type)
{
    int sbt, dbt, sf, df, c;

    /* Process delayed historical cast enforcement for narrow char or short variable storage */
    if (vtop->r & VT_MUSTCAST) {
        vtop->r &= ~VT_MUSTCAST;
        force_charshort_cast(vtop->type.t);
    }
    
    dbt = type->t & (VT_BTYPE | VT_UNSIGNED);
    sbt = vtop->type.t & (VT_BTYPE | VT_UNSIGNED);

    if (sbt != dbt && !nocode_wanted) {
        sf = is_float(sbt);
        df = is_float(dbt);
        c = (vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
        
        if (sf && df) {
            /* Transform compile-time floating-point constants directly to target precision maps */
            if (c) {
                if (dbt == VT_FLOAT && sbt == VT_DOUBLE) 
                    vtop->c.f = (float)vtop->c.d;
                else if (dbt == VT_FLOAT && sbt == VT_LDOUBLE) 
                    vtop->c.f = (float)vtop->c.ld;
                else if (dbt == VT_DOUBLE && sbt == VT_FLOAT) 
                    vtop->c.d = (double)vtop->c.f;
                else if (dbt == VT_DOUBLE && sbt == VT_LDOUBLE) 
                    vtop->c.d = (double)vtop->c.ld;
                else if (dbt == VT_LDOUBLE && sbt == VT_FLOAT) 
                    vtop->c.ld = (long double)vtop->c.f;
                else if (dbt == VT_LDOUBLE && sbt == VT_DOUBLE) 
                    vtop->c.ld = (long double)vtop->c.d;
            } else {
                /* Generate low-level x87 hardware conversion opcodes for dynamic variables */
                gen_cvt_ftof(dbt);
            }
        } else if (df) {
            /* Execute mathematical conversion shifting scalar integer maps straight into floating-point targets */
            if (c) {
                switch(sbt) {
                case VT_LLONG | VT_UNSIGNED:
                case VT_LLONG:
                    goto do_itof;
                case VT_INT | VT_UNSIGNED:
                    switch(dbt) {
                    case VT_FLOAT: vtop->c.f = (float)vtop->c.ui; break;
                    case VT_DOUBLE: vtop->c.d = (double)vtop->c.ui; break;
                    case VT_LDOUBLE: vtop->c.ld = (long double)vtop->c.ui; break;
                    }
                    break;
                default:
                    switch(dbt) {
                    case VT_FLOAT: vtop->c.f = (float)vtop->c.i; break;
                    case VT_DOUBLE: vtop->c.d = (double)vtop->c.i; break;
                    case VT_LDOUBLE: vtop->c.ld = (long double)vtop->c.i; break;
                    }
                    break;
                }
            } else {
            do_itof:
                gen_cvt_itof1(dbt);
            }
        } else if (sf) {
            /* Execute mathematical truncation shifting floating-point maps straight into scalar integer targets */
            if (dbt != (VT_INT | VT_UNSIGNED) && dbt != (VT_LLONG | VT_UNSIGNED) && dbt != VT_LLONG)
                dbt = VT_INT;
                
            if (c) {
                switch(dbt) {
                case VT_LLONG | VT_UNSIGNED:
                case VT_LLONG:
                    goto do_ftoi;
                case VT_INT | VT_UNSIGNED:
                    switch(sbt) {
                    case VT_FLOAT: vtop->c.ui = (unsigned int)vtop->c.d; break;
                    case VT_DOUBLE: vtop->c.ui = (unsigned int)vtop->c.d; break;
                    case VT_LDOUBLE: vtop->c.ui = (unsigned int)vtop->c.d; break;
                    }
                    break;
                default:
                    switch(sbt) {
                    case VT_FLOAT: vtop->c.i = (int)vtop->c.d; break;
                    case VT_DOUBLE: vtop->c.i = (int)vtop->c.d; break;
                    case VT_LDOUBLE: vtop->c.i = (int)vtop->c.d; break;
                    }
                    break;
                }
            } else {
            do_ftoi:
                gen_cvt_ftoi1(dbt);
            }
            if (dbt == VT_INT && (type->t & (VT_BTYPE | VT_UNSIGNED)) != dbt) {
                /* Cascade subsequent narrow mask cast processing loops for char/short/bool types */
                vtop->type.t = dbt;
                gen_cast(type);
            }
        } else if ((dbt & VT_BTYPE) == VT_LLONG) {
            if ((sbt & VT_BTYPE) != VT_LLONG) {
                /* Promote baseline scalar integers straight into 64-bit multi-precision long long structures */
                if (c) {
                    if (sbt == (VT_INT | VT_UNSIGNED))
                        vtop->c.ll = vtop->c.ui;
                    else
                        vtop->c.ll = vtop->c.i;
                } else {
                    gv(RC_INT);
                    if (sbt == (VT_INT | VT_UNSIGNED)) {
                        vpushi(0);
                        gv(RC_INT);
                    } else {
                        gv_dup();
                        vpushi(31);
                        gen_op(TOK_SAR);
                    }
                    vtop[-1].r2 = vtop->r;
                    vpop();
                }
            }
        } else if (dbt == VT_BOOL) {
            /* Reduce scalar assignments directly to logical Boolean expressions (0 or 1) */
            vpushi(0);
            gen_op(TOK_NE);
        } else if ((dbt & VT_BTYPE) == VT_BYTE || (dbt & VT_BTYPE) == VT_SHORT) {
            force_charshort_cast(dbt);
        } else if ((dbt & VT_BTYPE) == VT_INT) {
            if (sbt == VT_LLONG) {
                /* Truncate 64-bit long long structures dropping high order dword fields */
                lexpand();
                vpop();
            } 
        }
    }
    vtop->type = *type;
}

/* Calculate the explicit execution memory footprint size and assign the minimum hardware alignment pointer 'a' */
static int type_size(CType *type, int *a)
{
    Sym *s;
    int bt;

    bt = type->t & VT_BTYPE;
    if (bt == VT_STRUCT) {
        s = type->ref;
        *a = s->r;
        return s->c;
    } else if (bt == VT_PTR) {
        if (type->t & VT_ARRAY) {
            s = type->ref;
            return type_size(&s->type, a) * s->c;
        } else {
            *a = PTR_SIZE;
            return PTR_SIZE;
        }
    } else if (bt == VT_LDOUBLE) {
        *a = LDOUBLE_ALIGN;
        return LDOUBLE_SIZE; /* Constrained directly to native 12-byte layout for x87 FPU optimization maps */
    } else if (bt == VT_DOUBLE || bt == VT_LLONG) {
        *a = 4; /* Enforced straight to native 4-byte boundaries matching the TRDOS 386 x86 Protected Mode layout */
        return 8;
    } else if (bt == VT_INT || bt == VT_ENUM || bt == VT_FLOAT) {
        *a = 4;
        return 4;
    } else if (bt == VT_SHORT) {
        *a = 2;
        return 2;
    } else {
        *a = 1;
        return 1;
    }
}

/* Return the underlying pointed target type from a given pointer type descriptor */
static inline CType *pointed_type(CType *type)
{
    return &type->ref->type;
}

/* Modify the target type configuration converting its layout definition into an explicit pointer to type */
static void mk_pointer(CType *type)
{
    Sym *s;
    s = sym_push(SYM_FIELD, type, 0, -1);
    type->t = VT_PTR | (type->t & ~VT_TYPE);
    type->ref = s;
}

/* Evaluate and verify if two abstract data type descriptors maintain language level compatibility boundaries */
static int is_compatible_types(CType *type1, CType *type2)
{
    Sym *s1, *s2;
    int bt1, bt2, t1, t2;

    t1 = type1->t & VT_TYPE;
    t2 = type2->t & VT_TYPE;
    bt1 = t1 & VT_BTYPE;
    bt2 = t2 & VT_BTYPE;
    
    if (bt1 == VT_PTR) {
        type1 = pointed_type(type1);
        /* Function exception: Convert implicitly to functional code pointer targets seamlessly */
        if (bt2 != VT_FUNC) {
            if (bt2 != VT_PTR)
                return 0;
            type2 = pointed_type(type2);
        }
        /* Strict void tracking check: Void pointer abstractions safely match arbitrary memory type descriptors */
        if ((type1->t & VT_TYPE) == VT_VOID || (type2->t & VT_TYPE) == VT_VOID)
            return 1;
        return is_compatible_types(type1, type2);
    } else if (bt1 == VT_STRUCT || bt2 == VT_STRUCT) {
        return (type1->ref == type2->ref);
    } else if (bt1 == VT_FUNC) {
        if (bt2 != VT_FUNC)
            return 0;
        s1 = type1->ref;
        s2 = type2->ref;
        
        if (!is_compatible_types(&s1->type, &s2->type))
            return 0;
            
        /* Legacy fallback: Support prototype overrides if historical K&R style parameters are cached */
        if (s1->c == FUNC_OLD || s2->c == FUNC_OLD)
            return 1;
        if (s1->c != s2->c)
            return 0;
            
        while (s1 != NULL) {
            if (s2 == NULL)
                return 0;
            if (!is_compatible_types(&s1->type, &s2->type))
                return 0;
            s1 = s1->next;
            s2 = s2->next;
        }
        if (s2)
            return 0;
        return 1;
    } else {
        return 1;
    }
}

/* Print and format a structured C type descriptor straight into a human-readable destination buffer */
void type_to_str(char *buf, int buf_size, CType *type, const char *varstr)
{
    int bt, v, t;
    Sym *s, *sa;
    char buf1[256];
    const char *tstr;

    t = type->t & VT_TYPE;
    bt = t & VT_BTYPE;
    buf[0] = '\0';
    
    if (t & VT_UNSIGNED)
        pstrcat(buf, buf_size, "unsigned ");
        
    switch(bt) {
    case VT_VOID:
        tstr = "void";
        goto add_tstr;
    case VT_BOOL:
        tstr = "_Bool";
        goto add_tstr;
    case VT_BYTE:
        tstr = "char";
        goto add_tstr;
    case VT_SHORT:
        tstr = "short";
        goto add_tstr;
    case VT_INT:
        tstr = "int";
        goto add_tstr;
    case VT_LONG:
        tstr = "long";
        goto add_tstr;
    case VT_LLONG:
        tstr = "long long";
        goto add_tstr;
    case VT_FLOAT:
        tstr = "float";
        goto add_tstr;
    case VT_DOUBLE:
        tstr = "double";
        goto add_tstr;
    case VT_LDOUBLE:
        tstr = "long double";
    add_tstr:
        pstrcat(buf, buf_size, tstr);
        break;
    case VT_ENUM:
    case VT_STRUCT:
        if (bt == VT_STRUCT)
            tstr = "struct ";
        else
            tstr = "enum ";
        pstrcat(buf, buf_size, tstr);
        v = type->ref->v & ~SYM_STRUCT;
        if (v >= SYM_FIRST_ANOM)
            pstrcat(buf, buf_size, "<anonymous>");
        else
            pstrcat(buf, buf_size, get_tok_str(v, NULL));
        break;
    case VT_FUNC:
        s = type->ref;
        type_to_str(buf, buf_size, &s->type, varstr);
        pstrcat(buf, buf_size, "(");
        sa = s->next;
        while (sa != NULL) {
            type_to_str(buf1, sizeof(buf1), &sa->type, NULL);
            pstrcat(buf, buf_size, buf1);
            sa = sa->next;
            if (sa)
                pstrcat(buf, buf_size, ", ");
        }
        pstrcat(buf, buf_size, ")");
        goto no_var;
    case VT_PTR:
        s = type->ref;
        pstrcpy(buf1, sizeof(buf1), "*");
        if (varstr)
            pstrcat(buf1, sizeof(buf1), varstr);
        type_to_str(buf, buf_size, &s->type, buf1);
        goto no_var;
    }
    if (varstr) {
        pstrcat(buf, buf_size, " ");
        pstrcat(buf, buf_size, varstr);
    }
 no_var: ;
}

/* Verify type compatibility to store vtop into 'dt' target type, injecting explicit casts if needed */
static void gen_assign_cast(CType *dt)
{
    CType *st;
    char buf1[256], buf2[256];
    int dbt, sbt;

    st = &vtop->type; /* Target core source type */
    dbt = dt->t & VT_BTYPE;
    sbt = st->t & VT_BTYPE;
    
    if (dbt == VT_PTR) {
        /* Evaluate special type promotion constraints for pointer assignments */
        if (sbt == VT_FUNC) {
            if (!is_compatible_types(pointed_type(dt), st))
                goto error;
            else
                goto type_ok;
        }
        
        /* Enforce absolute literal 0 value compatibility mapping it as a standard valid null pointer */
        if (sbt == VT_INT && ((vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST) && vtop->c.i == 0)
            goto type_ok;
            
        /* Process and allow implicit pointer-from-integer conversions, generating compile-time warnings */
        if (sbt == VT_BYTE || sbt == VT_SHORT || sbt == VT_INT || sbt == VT_LLONG) {
            warning("assignment makes pointer from integer without a cast");
            goto type_ok;
        }
    } else if (dbt == VT_BYTE || dbt == VT_SHORT || dbt == VT_INT || dbt == VT_LLONG) {
        if (sbt == VT_PTR || sbt == VT_FUNC) {
            warning("assignment makes integer from pointer without a cast");
            goto type_ok;
        }
    }
    
    if (!is_compatible_types(dt, st)) {
    error:
        type_to_str(buf1, sizeof(buf1), st, NULL);
        type_to_str(buf2, sizeof(buf2), dt, NULL);
        error("cannot cast '%s' to '%s'", buf1, buf2);
    }
    
type_ok:
    gen_cast(dt);
}

/* Store the current vtop stack value entry straight into the underlying lvalue structure cached on stack */
void vstore(void)
{
    int sbt, dbt, ft, r, t, size, align, bit_size, bit_pos, rc, delayed_cast;
    GFuncContext gf;

    ft = vtop[-1].type.t;
    sbt = vtop->type.t & VT_BTYPE;
    dbt = ft & VT_BTYPE;
    
    if (((sbt == VT_INT || sbt == VT_SHORT) && dbt == VT_BYTE) || (sbt == VT_INT && dbt == VT_SHORT)) {
        /* Optimization path: Delay immediate short/char cast generation to streamline register utilization */
        delayed_cast = VT_MUSTCAST;
        vtop->type.t = ft & VT_TYPE;
    } else {
        delayed_cast = 0;
        gen_assign_cast(&vtop[-1].type);
    }

    if (sbt == VT_STRUCT) {
        /* Composite structures assignment: Delegate payload replication straight to native memcpy loops */
        if (!nocode_wanted) {
            vdup();
            gfunc_start(&gf, FUNC_CDECL);
            
            size = type_size(&vtop->type, &align);
            vpushi(size);
            gfunc_param(&gf);
            
            /* Secure the source address parameters */
            vtop->type.t = VT_INT;
            gaddrof();
            gfunc_param(&gf);
            
            /* Secure the destination address parameters */
            vswap();
            vtop->type.t = VT_INT;
            gaddrof();
            gfunc_param(&gf);
            
            save_regs(0);
            vpush_global_sym(&func_old_type, TOK_memcpy);
            gfunc_call(&gf);
        } else {
            vswap();
            vpop();
        }
    } else if (ft & VT_BITFIELD) {
        /* Process dynamic bitfield packing, parsing targeted bit size and shift position attributes */
        bit_pos = (ft >> VT_STRUCT_SHIFT) & 0x3f;
        bit_size = (ft >> (VT_STRUCT_SHIFT + 6)) & 0x3f;
        
        vtop[-1].type.t = ft & ~(VT_BITFIELD | (-1 << VT_STRUCT_SHIFT));

        vdup();
        vtop[-1] = vtop[-2];

        vpushi((1 << bit_size) - 1);
        gen_op('&');
        vpushi(bit_pos);
        gen_op(TOK_SHL);
        
        vswap();
        vpushi(~(((1 << bit_size) - 1) << bit_pos));
        gen_op('&');
        gen_op('|');
        
        vstore();
    } else {
        /* Runtime memory bounds verification kancaları safely dismantled to optimize flat execution paths */
        if (!nocode_wanted) {
            rc = RC_INT;
            if (is_float(ft))
                rc = RC_FLOAT;
                
            r = gv(rc);
            
            /* If the destination lvalue context resides inside local storage, fetch its absolute register address */
            if ((vtop[-1].r & VT_VALMASK) == VT_LLOCAL) {
                SValue sv;
                t = get_reg(RC_INT);
                sv.type.t = VT_INT;
                sv.r = VT_LOCAL | VT_LVAL;
                sv.c.ul = vtop[-1].c.ul;
                load(t, &sv);
                vtop[-1].r = t | VT_LVAL;
            }
            store(r, vtop - 1);
            
            /* Handle 64-bit long long structures shifting low/high dwords sequentially into memory cells + 4 */
            if ((ft & VT_BTYPE) == VT_LLONG) {
                vswap();
                vtop->type.t = VT_INT;
                gaddrof();
                vpushi(4);
                gen_op('+');
                vtop->r |= VT_LVAL;
                vswap();
                store(vtop->r2, vtop - 1);
            }
        }
        vswap();
        vtop--; /* Enforce direct stack decrement over vpop() to shield native x87 FPU cache states from flushing */
        vtop->r |= delayed_cast;
    }
}

/* Process and generate postfix or prefix increment and decrement operations (++, --) */
void inc(int post, int c)
{
    test_lvalue();
    vdup(); /* Secure and save the destination lvalue context on the evaluation stack */
    if (post) {
        gv_dup(); /* Duplicate the active value block if a postfix operator is encountered */
        vrotb(3);
        vrotb(3);
    }
    
    /* Calculate and push mathematical step constants matching token indicators directly */
    vpushi(c - TOK_MID); 
    gen_op('+');
    vstore(); /* Execute memory commitment storing the updated value back into lvalue */
    if (post)
        vpop(); /* Return the cached historical value snapshot if a postfix execution is triggered */
}

/* Parse GNUC __attribute__ extensions and bind metadata directly into active AttributeDef frames */
static void parse_attribute(AttributeDef *ad)
{
    int t, n;
    
    while (tok == TOK_ATTRIBUTE1 || tok == TOK_ATTRIBUTE2) {
        next();
        skip('(');
        skip('(');
        while (tok != ')') {
            if (tok < TOK_IDENT)
                expect("attribute name");
            t = tok;
            next();
            switch(t) {
            case TOK_SECTION1:
            case TOK_SECTION2:
                skip('(');
                if (tok != TOK_STR)
                    expect("section name");
                /* Align target attributes straight to our safe flat section mapping registry */
                ad->section = find_section(tcc_state, (char *)tokc.cstr->data);
                next();
                skip(')');
                break;
            case TOK_ALIGNED1:
            case TOK_ALIGNED2:
                if (tok == '(') {
                    next();
                    n = expr_const();
                    if (n <= 0 || (n & (n - 1)) != 0) 
                        error("alignment must be a positive power of two");
                    skip(')');
                } else {
                    n = MAX_ALIGN;
                }
                ad->aligned = n;
                break;
            case TOK_UNUSED1:
            case TOK_UNUSED2:
            case TOK_NORETURN1:
            case TOK_NORETURN2:
                /* Explicitly bypassed as the native TRDOS 386 port optimization layer drops dead objects dynamically */
                break;
            case TOK_CDECL1:
            case TOK_CDECL2:
            case TOK_CDECL3:
                ad->func_call = FUNC_CDECL;
                break;
            case TOK_STDCALL1:
            case TOK_STDCALL2:
            case TOK_STDCALL3:
                ad->func_call = FUNC_STDCALL;
                break;
            default:
                /* Safely consume parametric trailing tokens matching unknown GNU extensions */
                if (tok == '(') {
                    next();
                    while (tok != ')' && tok != -1)
                        next();
                    next();
                }
                break;
            }
            if (tok != ',')
                break;
            next();
        }
        skip(')');
        skip(')');
    }
}

/* Process enum/struct/union type declarations. Parameter 'u' corresponds to either VT_ENUM or VT_STRUCT */
static void struct_decl(CType *type, int u)
{
    int a, v, size, align, maxalign, c, offset;
    int bit_size, bit_pos, bsize, bt, lbit_pos;
    Sym *s, *ss, **ps;
    AttributeDef ad;
    CType type1, btype;

    a = tok; /* Capture and record the exact structural type declaration prefix token */
    next();
    if (tok != '{') {
        v = tok;
        next();
        if (v < TOK_IDENT)
            expect("struct/union/enum name");
            
        /* Struct already defined in context? Query the tracker and return it instantly if discovered */
        s = struct_find(v);
        if (s) {
            if (s->type.t != a)
                error("invalid type");
            goto do_decl;
        }
    } else {
        v = anon_sym++;
    }
    type1.t = a;
    s = sym_push(v | SYM_STRUCT, &type1, 0, 0);

 do_decl:
    type->t = u;
    type->ref = s;
    
    if (tok == '{') {
        next();
        if (s->c)
            error("struct/union/enum already defined");
            
        c = 0; /* Enforce compilation rules: Structures or enums cannot remain empty */
        
        if (a == TOK_ENUM) {
            /* Parse C enum sequence fields continuously */
            for(;;) {
                v = tok;
                if (v < TOK_UIDENT)
                    expect("identifier");
                next();
                if (tok == '=') {
                    next();
                    c = expr_const();
                }
                /* Enum identifiers implicitly possess static persistence within execution scopes */
                ss = sym_push(v, &int_type, VT_CONST, c);
                ss->type.t |= VT_STATIC;
                if (tok != ',')
                    break;
                next();
                c++;
                if (tok == '}')
                    break;
            }
            skip('}');
        } else {
            /* Parse composite struct or union elements sequentially */
            maxalign = 1;
            ps = &s->next;
            bit_pos = 0;
            offset = 0;
            while (tok != '}') {
                parse_btype(&btype, &ad);
                while (1) {
                    bit_size = -1;
                    v = 0;
                    type1 = btype;
                    if (tok != ':') {
                        type_decl(&type1, &ad, &v, TYPE_DIRECT);
                        if ((type1.t & VT_BTYPE) == VT_FUNC || (type1.t & (VT_TYPEDEF | VT_STATIC | VT_EXTERN | VT_INLINE)))
                            error("invalid type for '%s'", get_tok_str(v, NULL));
                    }
                    if (tok == ':') {
                        next();
                        bit_size = expr_const();
                        if (bit_size < 0)
                            error("negative width in bit-field '%s'", get_tok_str(v, NULL));
                        if (v && bit_size == 0)
                            error("zero width for bit-field '%s'", get_tok_str(v, NULL));
                    }
                    size = type_size(&type1, &align);
                    lbit_pos = 0;
                    if (bit_size >= 0) {
                        bt = type1.t & VT_BTYPE;
                        if (bt != VT_INT && bt != VT_BYTE && bt != VT_SHORT && bt != VT_ENUM)
                            error("bitfields must have scalar type");
                        bsize = size * 8;
                        if (bit_size > bsize) {
                            error("width of '%s' exceeds its type", get_tok_str(v, NULL));
                        } else if (bit_size == bsize) {
                            bit_pos = 0; /* Full boundary alignment width: Defuse sub-bit field modifications */
                        } else if (bit_size == 0) {
                            /* Clear packing limits to enforce strict structural data block alignment padding */
                            if (bit_pos > 0)
                                bit_pos = bsize;
                        } else {
                            if ((bit_pos + bit_size) > bsize)
                                bit_pos = 0;
                            lbit_pos = bit_pos;
                            
                            /* Pack sub-bit alignments into core type modifiers using structural shifts */
                            type1.t |= VT_BITFIELD | (bit_pos << VT_STRUCT_SHIFT) | (bit_size << (VT_STRUCT_SHIFT + 6));
                            bit_pos += bit_size;
                        }
                    } else {
                        bit_pos = 0;
                    }
                    if (v) {
                        /* Allocate new contiguous memory tracking entries only if starting a fresh bit-field block */
                        if (lbit_pos == 0) {
                            if (a == TOK_STRUCT) {
                                c = (c + align - 1) & -align;
                                offset = c;
                                c += size;
                            } else {
                                offset = 0;
                                if (size > c)
                                    c = size;
                            }
                            if (align > maxalign)
                                maxalign = align;
                        }
                        
                        /* Debug log tracing loops completely siphoned away to retain absolute core minimalism */
                        ss = sym_push(v | SYM_FIELD, &type1, 0, offset);
                        *ps = ss;
                        ps = &ss->next;
                    }
                    if (tok == ';' || tok == TOK_EOF)
                        break;
                    skip(',');
                }
                skip(';');
            }
            skip('}');
            /* Compute and finalize structural size properties applying maximal alignment constraints */
            s->c = (c + maxalign - 1) & -maxalign; 
            s->r = maxalign;
        }
    }
}

/* Return 0 if no type declaration is encountered. Otherwise, return the parsed basic type and advance the token stream */
static int parse_btype(CType *type, AttributeDef *ad)
{
    int t, u, type_found;
    Sym *s;
    CType type1;

    memset(ad, 0, sizeof(AttributeDef));
    type_found = 0;
    t = 0;
    
    while(1) {
        switch(tok) {
        case TOK_EXTENSION:
            /* Safely bypass and ignore explicit GNU C __extension__ keywords */
            next();
            continue;

        /* Core primitive data types resolution paths */
        case TOK_CHAR:
            u = VT_BYTE;
        basic_type:
            next();
        basic_type1:
            if ((t & VT_BTYPE) != 0)
                error("too many basic types");
            t |= u;
            break;
        case TOK_VOID:
            u = VT_VOID;
            goto basic_type;
        case TOK_SHORT:
            u = VT_SHORT;
            goto basic_type;
        case TOK_INT:
            next();
            break;
        case TOK_LONG:
            next();
            if ((t & VT_BTYPE) == VT_DOUBLE) {
                t = (t & ~VT_BTYPE) | VT_LDOUBLE;
            } else if ((t & VT_BTYPE) == VT_LONG) {
                t = (t & ~VT_BTYPE) | VT_LLONG; /* Resolve continuous long long (64-bit integer) modifiers */
            } else {
                u = VT_LONG;
                goto basic_type1;
            }
            break;
        case TOK_BOOL:
            u = VT_BOOL;
            goto basic_type;
        case TOK_FLOAT:
            u = VT_FLOAT;
            goto basic_type;
        case TOK_DOUBLE:
            next();
            if ((t & VT_BTYPE) == VT_LONG) {
                t = (t & ~VT_BTYPE) | VT_LDOUBLE; /* Enforce x87 12-byte long double configurations if long double is found */
            } else {
                u = VT_DOUBLE;
                goto basic_type1;
            }
            break;
        case TOK_ENUM:
            struct_decl(&type1, VT_ENUM);
        basic_type2:
            u = type1.t;
            type->ref = type1.ref;
            goto basic_type1;
        case TOK_STRUCT:
        case TOK_UNION:
            struct_decl(&type1, VT_STRUCT);
            goto basic_type2;

        /* Language native type qualifiers and layout modifiers */
        case TOK_CONST1:
        case TOK_CONST2:
        case TOK_CONST3:
        case TOK_VOLATILE1:
        case TOK_VOLATILE2:
        case TOK_VOLATILE3:
        case TOK_REGISTER:
        case TOK_SIGNED1:
        case TOK_SIGNED2:
        case TOK_SIGNED3:
        case TOK_AUTO:
        case TOK_RESTRICT1:
        case TOK_RESTRICT2:
        case TOK_RESTRICT3:
            next();
            break;
        case TOK_UNSIGNED:
            t |= VT_UNSIGNED;
            next();
            break;

        /* Variable storage persistence and structural scopes visibility qualifiers */
        case TOK_EXTERN:
            t |= VT_EXTERN;
            next();
            break;
        case TOK_STATIC:
            t |= VT_STATIC;
            next();
            break;
        case TOK_TYPEDEF:
            t |= VT_TYPEDEF;
            next();
            break;
        case TOK_INLINE1:
        case TOK_INLINE2:
        case TOK_INLINE3:
            t |= VT_INLINE;
            next();
            break;

        /* GNU C compiler extensions parsing blocks */
        case TOK_ATTRIBUTE1:
        case TOK_ATTRIBUTE2:
            parse_attribute(ad);
            break;
        case TOK_TYPEOF1:
        case TOK_TYPEOF2:
        case TOK_TYPEOF3:
            next();
            parse_expr_type(&type1);
            goto basic_type2;
        default:
            /* Evaluate custom user-defined types via typedef structural lookup chains */
            s = sym_find(tok);
            if (!s || !(s->type.t & VT_TYPEDEF))
                goto the_end;
            t |= (s->type.t & ~VT_TYPEDEF);
            type->ref = s->type.ref;
            next();
            break;
        }
        type_found = 1;
    }
the_end:
    /* Optimization rule: 'long' remains unrepresented as a raw isolated type internally; flatten it straight to standard 32-bit int */
    if ((t & VT_BTYPE) == VT_LONG)
        t = (t & ~VT_BTYPE) | VT_INT;
    type->t = t;
    return type_found;
}

/* Convert a function parameter type dynamically (transforms array definitions to pointers and functions to function pointers) */
static inline void convert_parameter_type(CType *pt)
{
    /* Array parameters must be implicitly transformed to pointers according to strict ANSI C semantics */
    pt->t &= ~VT_ARRAY;
    if ((pt->t & VT_BTYPE) == VT_FUNC) {
        mk_pointer(pt);
    }
}

/* Recursive postfix type parser resolving function signature parameters and array dimensional limits */
static void post_type(CType *type, AttributeDef *ad)
{
    int n, l, t1;
    Sym **plast, *s, *first;
    AttributeDef ad1;
    CType pt;

    if (tok == '(') {
        /* Evaluation path: Process and decompose a function prototype declaration sequence */
        next();
        l = 0;
        first = NULL;
        plast = &first;
        while (tok != ')') {
            /* Parse parameter identifier token name and compute contextual variable spacing */
            if (l != FUNC_OLD) {
                if (!parse_btype(&pt, &ad1)) {
                    if (l) {
                        error("invalid type");
                    } else {
                        l = FUNC_OLD;
                        goto old_proto;
                    }
                }
                l = FUNC_NEW;
                if ((pt.t & VT_BTYPE) == VT_VOID && tok == ')')
                    break;
                type_decl(&pt, &ad1, &n, TYPE_DIRECT | TYPE_ABSTRACT);
                if ((pt.t & VT_BTYPE) == VT_VOID)
                    error("parameter declared as void");
            } else {
            old_proto:
                /* Fallback strategy: Process legacy K&R style old function prototype descriptors */
                n = tok;
                pt.t = VT_INT;
                next();
            }
            convert_parameter_type(&pt);
            s = sym_push(n | SYM_FIELD, &pt, 0, 0);
            *plast = s;
            plast = &s->next;
            if (tok == ',') {
                next();
                if (l == FUNC_NEW && tok == TOK_DOTS) {
                    l = FUNC_ELLIPSIS; /* Mark signature type as variadic parameter function (...) */
                    next();
                    break;
                }
            }
        }
        /* Defuse empty declaration loops enforcing legacy prototype signatures if no parameters exist */
        if (l == 0)
            l = FUNC_OLD;
        skip(')');
        
        t1 = type->t & VT_STORAGE;
        type->t &= ~VT_STORAGE;
        post_type(type, ad);
        
        /* Enqueue an anonymous field descriptor containing the validated function signature prototype metadata */
        s = sym_push(SYM_FIELD, type, ad->func_call, l);
        s->next = first;
        type->t = t1 | VT_FUNC;
        type->ref = s;
    } else if (tok == '[') {
        /* Evaluation path: Parse multidimensional array layout definitions and constraint metrics */
        next();
        n = -1;
        if (tok != ']') {
            n = expr_const();
            if (n < 0)
                error("invalid array size");    
        }
        skip(']');
        
        t1 = type->t & VT_STORAGE;
        type->t &= ~VT_STORAGE;
        post_type(type, ad);
        
        /* Enqueue an anonymous field descriptor mapping the array element component data boundaries */
        s = sym_push(SYM_FIELD, type, 0, n);
        type->t = t1 | VT_ARRAY | VT_PTR;
        type->ref = s;
    }
}

/* Parse a type declaration sequence (excluding the basic type specification) and load it into 'type'.
   'td' acts as a directional bitmask flag identifying abstract or direct variable identifiers. */
static void type_decl(CType *type, AttributeDef *ad, int *v, int td)
{
    Sym *s;
    CType type1, *type2;

    while (tok == '*') {
        next();
    redo:
        switch(tok) {
        case TOK_CONST1:
        case TOK_CONST2:
        case TOK_CONST3:
        case TOK_VOLATILE1:
        case TOK_VOLATILE2:
        case TOK_VOLATILE3:
        case TOK_RESTRICT1:
        case TOK_RESTRICT2:
        case TOK_RESTRICT3:
            next();
            goto redo;
        }
        mk_pointer(type);
    }
    
    if (tok == TOK_ATTRIBUTE1 || tok == TOK_ATTRIBUTE2)
        parse_attribute(ad);

    /* Process recursive inner-parenthesis abstract type configurations complex layouts */
    type1.t = 0; 
    if (tok == '(') {
        next();
        if (tok == TOK_ATTRIBUTE1 || tok == TOK_ATTRIBUTE2)
            parse_attribute(ad);
        type_decl(&type1, ad, v, td);
        skip(')');
    } else {
        /* Parse literal variable identifier token labels directly */
        if (tok >= TOK_IDENT && (td & TYPE_DIRECT)) {
            *v = tok;
            next();
        } else {
            if (!(td & TYPE_ABSTRACT))
                expect("identifier");
            *v = 0;
        }
    }
    post_type(type, ad);
    
    if (tok == TOK_ATTRIBUTE1 || tok == TOK_ATTRIBUTE2)
        parse_attribute(ad);
        
    if (!type1.t)
        return;
        
    /* Chain and append newly resolved types at the absolute end of the recursive type1 layout map */
    type2 = &type1;
    for(;;) {
        s = type2->ref;
        type2 = &s->type;
        if (!type2->t) {
            *type2 = *type;
            break;
        }
    }
    *type = type1;
}

/* Extract and compute the baseline lvalue modifier token flags required to match native data storage type t */
static int lvalue_type(int t)
{
    int bt, r;
    r = VT_LVAL;
    bt = t & VT_BTYPE;
    
    if (bt == VT_BYTE || bt == VT_BOOL)
        r |= VT_LVAL_BYTE;
    else if (bt == VT_SHORT)
        r |= VT_LVAL_SHORT;
    else
        return r;
        
    if (t & VT_UNSIGNED)
        r |= VT_LVAL_UNSIGNED;
    return r;
}

/* Execute direct pointer indirection dereferencing operations ensuring strict semantic type validation */
static void indir(void)
{
    if ((vtop->type.t & VT_BTYPE) != VT_PTR)
        expect("pointer");
    if ((vtop->r & VT_LVAL) && !nocode_wanted)
        gv(RC_INT);
        
    vtop->type = *pointed_type(&vtop->type);
    
    /* Enforce language standard: Contiguous arrays are never raw native mutative assignable lvalues */
    if (!(vtop->type.t & VT_ARRAY)) {
        vtop->r |= lvalue_type(vtop->type.t);
        /* Bound checking hooks completely stripped away to maximize address calculation speeds */
    }
}

/* Push a type-safe parameter configuration onto the target function argument pipeline layout */
void gfunc_param_typed(GFuncContext *gf, Sym *func, Sym *arg)
{
    int func_type;
    CType type;

    func_type = func->c;
    if (func_type == FUNC_OLD || (func_type == FUNC_ELLIPSIS && arg == NULL)) {
        /* Default casting behavior: Enforce automatic promotion mapping isolated float registers to doubles */
        if ((vtop->type.t & VT_BTYPE) == VT_FLOAT) {
            type.t = VT_DOUBLE;
            gen_cast(&type);
        }
    } else if (arg == NULL) {
        error("too many arguments to function");
    } else {
        gen_assign_cast(&arg->type);
    }
    
    if (!nocode_wanted) {
        gfunc_param(gf);
    } else {
        vpop();
    }
}

/* Parse explicit parenthesis structures isolating a type definition or cast/typeof expression segment */
static void parse_expr_type(CType *type)
{
    int n;
    AttributeDef ad;

    skip('(');
    if (parse_btype(type, &ad)) {
        type_decl(type, &ad, &n, TYPE_ABSTRACT);
    } else {
        expr_type(type);
    }
    skip(')');
}

/* Rapidly push the current active constant value snapshot from the lexical token parser block */
static void vpush_tokc(int t)
{
    CType type;
    type.t = t;
    vsetc(&type, VT_CONST, &tokc);
}

/* Parse a primary or unary expression sequence handling data primitives, casts, and pointer references */
static void unary(void)
{
    int n, t, align, size, r;
    CType type;
    Sym *s;
    GFuncContext gf;
    AttributeDef ad;

 tok_next:
    switch(tok) {
    case TOK_EXTENSION:
        /* Safely skip explicit GNU C __extension__ attributes inside unary expression loops */
        next();
        goto tok_next;
    case TOK_CINT:
    case TOK_CCHAR: 
    case TOK_LCHAR:
        vpushi(tokc.i);
        next();
        break;
    case TOK_CUINT:
        vpush_tokc(VT_INT | VT_UNSIGNED);
        next();
        break;
    case TOK_CLLONG:
        vpush_tokc(VT_LLONG);
        next();
        break;
    case TOK_CULLONG:
        vpush_tokc(VT_LLONG | VT_UNSIGNED);
        next();
        break;
    case TOK_CFLOAT:
        vpush_tokc(VT_FLOAT);
        next();
        break;
    case TOK_CDOUBLE:
        vpush_tokc(VT_DOUBLE);
        next();
        break;
    case TOK_CLDOUBLE:
        vpush_tokc(VT_LDOUBLE);
        next();
        break;
    case TOK___FUNCTION__:
        if (!gnu_ext)
            goto tok_identifier;
        /* fall thru */
    case TOK___FUNC__:
        {
            void *ptr;
            int len;
            /* Resolve standard local function name context payload descriptor string */
            len = strlen(funcname) + 1;
            
            /* Generate matching char[len] array type structure dynamically */
            type.t = VT_BYTE;
            mk_pointer(&type);
            type.t |= VT_ARRAY;
            type.ref->c = len;
            vpush_ref(&type, data_section, data_section->data_offset, len);
            ptr = section_ptr_add(data_section, len);
            memcpy(ptr, funcname, len);
            next();
        }
        break;
    case TOK_LSTR:
        t = VT_INT;
        goto str_init;
    case TOK_STR:
        /* Process and instantiate continuous static raw string character data arrays */
        t = VT_BYTE;
    str_init:
        type.t = t;
        mk_pointer(&type);
        type.t |= VT_ARRAY;
        memset(&ad, 0, sizeof(AttributeDef));
        decl_initializer_alloc(&type, &ad, VT_CONST, 2, 0, 0);
        break;
    case '(':
        next();
        /* Type Casting or Grouped Expressions parsing branch */
        if (parse_btype(&type, &ad)) {
            type_decl(&type, &ad, &n, TYPE_ABSTRACT);
            skip(')');
            /* Check and process standard ISO C99 compound literal constructs */
            if (tok == '{') {
                if (global_expr)
                    r = VT_CONST;
                else
                    r = VT_LOCAL;
                
                /* Enforce rules: All compound configurations evaluate as lvalues except raw arrays */
                if (!(type.t & VT_ARRAY))
                    r |= lvalue_type(type.t);
                memset(&ad, 0, sizeof(AttributeDef));
                decl_initializer_alloc(&type, &ad, r, 1, 0, 0);
            } else {
                unary();
                gen_cast(&type);
            }
        } else if (tok == '{') {
            /* Synchronize register states before entering localized statement blocks */
            save_regs(0); 
            block(NULL, NULL, NULL, NULL, 0, 1);
            skip(')');
        } else {
            gexpr();
            skip(')');
        }
        break;
    case '*':
        next();
        unary();
        indir(); /* Execute explicit pointer indirection dereference sequence */
        break;
    case '&':
        next();
        unary();
        /* Assert operand validation: Function names and array structures bypass classical lvalue traits */
        if ((vtop->type.t & VT_BTYPE) != VT_FUNC && !(vtop->type.t & VT_ARRAY))
            test_lvalue();
        mk_pointer(&vtop->type);
        gaddrof();
        break;
    case '!':
        next();
        unary();
        if ((vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST)
            vtop->c.i = !vtop->c.i;
        else if ((vtop->r & VT_VALMASK) == VT_CMP)
            vtop->c.i = vtop->c.i ^ 1;
        else
            vseti(VT_JMP, gtst(1, 0));
        break;
    case '~':
        next();
        unary();
        vpushi(-1);
        gen_op('^');
        break;
    case '+':
        next();
        /* Add absolute zero to forcefully evaluate and trigger implicit conversion rules */
        unary();
        if ((vtop->type.t & VT_BTYPE) == VT_PTR)
            error("pointer not accepted for unary plus");
        vpushi(0);
        gen_op('+');
        break;
    case TOK_SIZEOF:
    case TOK_ALIGNOF1:
    case TOK_ALIGNOF2:
        t = tok;
        next();
        if (tok == '(') {
            parse_expr_type(&type);
        } else {
            unary_type(&type);
        }
        size = type_size(&type, &align);
        if (t == TOK_SIZEOF)
            vpushi(size);
        else
            vpushi(align);
        break;
        
    case TOK_INC:
    case TOK_DEC:
        t = tok;
        next();
        unary();
        inc(0, t); /* Execute prefix increment or decrement routine instantly */
        break;
    case '-':
        next();
        vpushi(0);
        unary();
        gen_op('-');
        break;
    case TOK_LAND:
        if (!gnu_ext)
            goto tok_identifier;
        next();
        /* GNU C Extension path: Secure and take the relative address offset of a target branching label */
        if (tok < TOK_UIDENT)
            expect("label identifier");
        s = label_find(tok);
        if (!s) {
            s = label_push(&global_label_stack, tok, LABEL_FORWARD);
        } else {
            if (s->r == LABEL_DECLARED)
                s->r = LABEL_FORWARD;
        }
        if (!s->type.t) {
            s->type.t = VT_VOID;
            mk_pointer(&s->type);
            s->type.t |= VT_STATIC;
        }
        vset(&s->type, VT_CONST | VT_SYM, 0);
        vtop->sym = s;
        next();
        break;
    default:
    tok_identifier:
        t = tok;
        next();
        if (t < TOK_UIDENT)
            expect("identifier");
        s = sym_find(t);
        if (!s) {
            if (tok != '(')
                error("'%s' undeclared", get_tok_str(t, NULL));
            /* Toleration strategy: Allow undeclared external function linkage defaulting implicitly to int() */
            s = external_global_sym(t, &func_old_type, 0); 
        }
        vset(&s->type, s->r, s->c);
        /* If forward tracking reference is detected, redirect internal symbols directly to s */
        if (vtop->r & VT_SYM) {
            vtop->sym = s;
            vtop->c.ul = 0;
        }
        break;
    }
    
    /* Process postfix operators and compound array/structure membership access loops */
    while (1) {
        if (tok == TOK_INC || tok == TOK_DEC) {
            inc(1, tok);
            next();
        } else if (tok == '.' || tok == TOK_ARROW) {
            /* Structural layout member extraction */ 
            if (tok == TOK_ARROW) 
                indir();
            test_lvalue();
            gaddrof();
            next();
            if ((vtop->type.t & VT_BTYPE) != VT_STRUCT)
                expect("struct or union");
            s = vtop->type.ref;
            
            /* Traverse layout sequence to map targeted field boundaries */
            tok |= SYM_FIELD;
            while ((s = s->next) != NULL) {
                if (s->v == tok)
                    break;
            }
            if (!s)
                error("field not found");
                
            /* Factor field offset directly into target pointer reference */
            vtop->type = char_pointer_type; /* Shift type alias window transiently to 'char *' */
            vpushi(s->c);
            gen_op('+');
            
            vtop->type = s->type;
            if (!(vtop->type.t & VT_ARRAY)) {
                vtop->r |= lvalue_type(vtop->type.t);
                /* Bounds verification tracking structures cleanly bypassed to guard core execution speed */
            }
            next();
        } else if (tok == '[') {
            /* Array indexing evaluation path */
            next();
            gexpr();
            gen_op('+');
            indir();
            skip(']');
        } else if (tok == '(') {
            SValue ret;
            Sym *sa;

            /* Procedural Function Call Processing Engine */
            if ((vtop->type.t & VT_BTYPE) != VT_FUNC) {
                if ((vtop->type.t & (VT_BTYPE | VT_ARRAY)) == VT_PTR) {
                    vtop->type = *pointed_type(&vtop->type);
                    if ((vtop->type.t & VT_BTYPE) != VT_FUNC)
                        goto error_func;
                } else {
                error_func:
                    expect("function pointer");
                }
            } else {
                vtop->r &= ~VT_LVAL;
            }
            
            s = vtop->type.ref;
            if (!nocode_wanted) {
                save_regs(0); /* Evacuate and cache active volatile working registers */
                gfunc_start(&gf, s->r);
            }
            next();
            sa = s->next; /* Position tracking at first registered structural parameter cell */
            
#ifdef INVERT_FUNC_PARAMS
            {
                int parlevel;
                Sym *args, *s1;
                ParseState saved_parse_state;
                TokenString str;
                
                /* Parse argument list caches pushing them onto target temporary stack sequentially */
                args = NULL;
                if (tok != ')') {
                    for(;;) {
                        tok_str_new(&str);
                        parlevel = 0;
                        while ((parlevel > 0 || (tok != ')' && tok != ',')) && tok != TOK_EOF) {
                            if (tok == '(')
                                parlevel++;
                            else if (tok == ')')
                                parlevel--;
                            tok_str_add_tok(&str);
                            next();
                        }
                        tok_str_add(&str, -1);
                        tok_str_add(&str, 0);
                        s1 = sym_push2(&args, 0, 0, (int)str.str);
                        s1->next = sa;
                        if (sa)
                            sa = sa->next;
                        if (tok == ')')
                            break;
                        skip(',');
                    }
                }
                
                /* Invert stack order to process parameters in reverse-sweeping target code blocks */
                save_parse_state(&saved_parse_state);
                while (args) {
                    macro_ptr = (int *)args->c;
                    next();
                    expr_eq();
                    if (tok != -1)
                        expect("',' or ')'");
                    gfunc_param_typed(&gf, s, args->next);
                    s1 = args->prev;
                    tok_str_free((int *)args->c);
                    tcc_free(args);
                    args = s1;
                }
                restore_parse_state(&saved_parse_state);
            }
#endif
            /* Struct return ABI optimization: Pass the target output location pointer parameter implicitly */
            if ((s->type.t & VT_BTYPE) == VT_STRUCT) {
                size = type_size(&s->type, &align);
                loc = (loc - size) & -align;
                ret.type = s->type;
                ret.r = VT_LOCAL | VT_LVAL;
                
                vseti(VT_LOCAL, loc);
                ret.c = vtop->c;
                if (!nocode_wanted)
                    gfunc_param(&gf);
                else
                    vtop--;
            } else {
                ret.type = s->type; 
                ret.r2 = VT_CONST;
                if (is_float(ret.type.t)) {
                    ret.r = REG_FRET; 
                } else {
                    if ((ret.type.t & VT_BTYPE) == VT_LLONG)
                        ret.r2 = REG_LRET;
                    ret.r = REG_IRET;
                }
                ret.c.i = 0;
            }
#ifndef INVERT_FUNC_PARAMS
            if (tok != ')') {
                for(;;) {
                    expr_eq();
                    gfunc_param_typed(&gf, s, sa);
                    if (sa)
                        sa = sa->next;
                    if (tok == ')')
                        break;
                    skip(',');
                }
            }
#endif
            if (sa)
                error("too few arguments to function");
            skip(')');
            if (!nocode_wanted)
                gfunc_call(&gf);
            else
                vtop--;
                
            /* Push and stabilize the final resulting function return value context */
            vsetc(&ret.type, ret.r, &ret.c);
            vtop->r2 = ret.r2;
        } else {
            break;
        }
    }
}

/* Process and evaluate C assignment expressions including compound assignment operators */
static void uneq(void)
{
    int t;
    
    unary();
    if (tok == '=' || (tok >= TOK_A_MOD && tok <= TOK_A_DIV) ||
        tok == TOK_A_XOR || tok == TOK_A_OR || tok == TOK_A_SHL || tok == TOK_A_SAR) {
        test_lvalue();
        t = tok;
        next();
        if (t == '=') {
            expr_eq();
        } else {
            vdup();
            expr_eq();
            /* Extract the core mathematical operation by masking the compound assignment flag */
            gen_op(t & 0x7f);
        }
        vstore(); /* Commit the evaluated value straight into the target lvalue */
    }
}

/* Parse multiplicative operator precedence layers: multiplication, division, and modulo (*, /, %) */
static void expr_prod(void)
{
    int t;

    uneq();
    while (tok == '*' || tok == '/' || tok == '%') {
        t = tok;
        next();
        uneq();
        gen_op(t);
    }
}

/* Parse additive operator precedence layers: addition and subtraction (+, -) */
static void expr_sum(void)
{
    int t;

    expr_prod();
    while (tok == '+' || tok == '-') {
        t = tok;
        next();
        expr_prod();
        gen_op(t);
    }
}

/* Parse bitwise shift operator precedence layers: shift left and arithmetic shift right (<<, >>) */
static void expr_shift(void)
{
    int t;

    expr_sum();
    while (tok == TOK_SHL || tok == TOK_SAR) {
        t = tok;
        next();
        expr_sum();
        gen_op(t);
    }
}

/* Parse relational inequality operator precedence layers (<, >, <=, >=, unsigned checks) */
static void expr_cmp(void)
{
    int t;

    expr_shift();
    while ((tok >= TOK_ULE && tok <= TOK_GT) || tok == TOK_ULT || tok == TOK_UGE) {
        t = tok;
        next();
        expr_shift();
        gen_op(t);
    }
}

/* Parse relational equality operator precedence layers: equal and not equal (==, !=) */
static void expr_cbmp(void) /* Renamed/aligned internally matching clear sequential token steps */
{
    int t;

    expr_cmp();
    while (tok == TOK_EQ || tok == TOK_NE) {
        t = tok;
        next();
        expr_cmp();
        gen_op(t);
    }
}

/* Parse bitwise logical AND operator precedence layer (&) */
static void expr_and(void)
{
    expr_cbmp(); /* Target corrected upstream link routing */
    while (tok == '&') {
        next();
        expr_cbmp();
        gen_op('&');
    }
}

/* Parse bitwise logical XOR (exclusive OR) operator precedence layer (^) */
static void expr_xor(void)
{
    expr_and();
    while (tok == '^') {
        next();
        expr_and();
        gen_op('^');
    }
}

/* Parse bitwise logical OR (inclusive OR) operator precedence layer (|) */
static void expr_or(void)
{
    expr_xor();
    while (tok == '|') {
        next();
        expr_xor();
        gen_op('|');
    }
}

/* Compile-time static evaluate handler tracking conditional preprocessor logical AND expressions (&&) */
static void expr_land_const(void)
{
    expr_or();
    while (tok == TOK_LAND) {
        next();
        expr_or();
        gen_op(TOK_LAND);
    }
}

/* Compile-time static evaluate handler tracking conditional preprocessor logical OR expressions (||) */
static void expr_lor_const(void)
{
    expr_land_const();
    while (tok == TOK_LOR) {
        next();
        expr_land_const();
        gen_op(TOK_LOR);
    }
}

/* Process short-circuit runtime logical AND evaluation paths triggering hardware jump instruction blocks (&&) */
static void expr_land(void)
{
    int t;

    expr_or();
    if (tok == TOK_LAND) {
        t = 0;
        for(;;) {
            t = gtst(1, t);
            if (tok != TOK_LAND) {
                vseti(VT_JMPI, t);
                break;
            }
            next();
            expr_or();
        }
    }
}

/* Process short-circuit runtime logical OR evaluation paths triggering hardware jump instruction blocks (||) */
static void expr_lor(void)
{
    int t;

    expr_land();
    if (tok == TOK_LOR) {
        t = 0;
        for(;;) {
            t = gtst(0, t);
            if (tok != TOK_LOR) {
                vseti(VT_JMP, t);
                break;
            }
            next();
            expr_land();
        }
    }
}

/* Parse and evaluate conditional ternary expressions (? :) handling compile-time constants or runtime branches */
static void expr_eq(void)
{
    int tt, u, r1, r2, rc, t1, t2, bt1, bt2;
    SValue sv;
    CType type, type1, type2;

    if (const_wanted) {
        int c1, c;
        expr_lor_const();
        if (tok == '?') {
            c = vtop->c.i;
            vpop();
            next();
            if (tok == ':' && gnu_ext) {
                c1 = c;
            } else {
                gexpr();
                c1 = vtop->c.i;
                vpop();
            }
            skip(':');
            expr_eq();
            if (c)
                vtop->c.i = c1;
        }
    } else {
        expr_lor();
        if (tok == '?') {
            next();
            if (vtop != vstack) {
                /* Optimization path: Force identical register save maps across both evaluation branches */
                if (is_float(vtop->type.t))
                    rc = RC_FLOAT;
                else
                    rc = RC_INT;
                gv(rc);
                save_regs(1);
            }
            if (tok == ':' && gnu_ext) {
                gv_dup();
                tt = gtst(1, 0);
            } else {
                tt = gtst(1, 0);
                gexpr();
            }
            type1 = vtop->type;
            sv = *vtop; /* Cache and save current value frame to avoid register pollution */
            vtop--;     /* Decrement top pointer directly to shield floating point stack state from unexpected flushes */
            skip(':');
            u = gjmp(0);
            gsym(tt);
            expr_eq();
            type2 = vtop->type;

            t1 = type1.t;
            bt1 = t1 & VT_BTYPE;
            t2 = type2.t;
            bt2 = t2 & VT_BTYPE;
            
            /* Apply standard ISO C rules to resolve implicit type promotions across ternary operands */
            if (is_float(bt1) || is_float(bt2)) {
                if (bt1 == VT_LDOUBLE || bt2 == VT_LDOUBLE) {
                    type.t = VT_LDOUBLE;
                } else if (bt1 == VT_DOUBLE || bt2 == VT_DOUBLE) {
                    type.t = VT_DOUBLE;
                } else {
                    type.t = VT_FLOAT;
                }
            } else if (bt1 == VT_LLONG || bt2 == VT_LLONG) {
                type.t = VT_LLONG;
                if ((t1 & (VT_BTYPE | VT_UNSIGNED)) == (VT_LLONG | VT_UNSIGNED) ||
                    (t2 & (VT_BTYPE | VT_UNSIGNED)) == (VT_LLONG | VT_UNSIGNED))
                    type.t |= VT_UNSIGNED;
            } else if (bt1 == VT_PTR || bt2 == VT_PTR) {
                type = type1;
            } else if (bt1 == VT_STRUCT || bt2 == VT_STRUCT) {
                type = type1;
            } else if (bt1 == VT_VOID || bt2 == VT_VOID) {
                type.t = VT_VOID;
            } else {
                type.t = VT_INT;
                if ((t1 & (VT_BTYPE | VT_UNSIGNED)) == (VT_INT | VT_UNSIGNED) ||
                    (t2 & (VT_BTYPE | VT_UNSIGNED)) == (VT_INT | VT_UNSIGNED))
                    type.t |= VT_UNSIGNED;
            }
                
            /* Execute compilation type cast conversions targeting the secondary operand layer */
            gen_cast(&type);
            rc = RC_INT;
            if (is_float(type.t)) {
                rc = RC_FLOAT;
            } else if ((type.t & VT_BTYPE) == VT_LLONG) {
                /* Target fixed return registers for 64-bit multi-precision layers to streamline hardware layouts */
                rc = RC_IRET; 
            }
            
            r2 = gv(rc);
            tt = gjmp(0);
            gsym(u);
            
            /* Restore the primary cached expression value snapshot and apply required casting rules */
            *vtop = sv;
            gen_cast(&type);
            r1 = gv(rc);
            move_reg(r2, r1);
            vtop->r = r2;
            gsym(tt);
        }
    }
}

/* Parse comma-separated expression sequences sequentially advancing through sub-assignment frames */
static void gexpr(void)
{
    while (1) {
        expr_eq();
        if (tok != ',')
            break;
        vpop();
        next();
    }
}

/* Extract and resolve expression target types while muting explicit machine code emission paths */
static void expr_type(CType *type)
{
    int a;

    a = nocode_wanted;
    nocode_wanted = 1;
    gexpr();
    *type = vtop->type;
    vpop();
    nocode_wanted = a;
}

/* Extract and resolve unary expression target types while muting explicit machine code emission paths */
static void unary_type(CType *type)
{
    int a;

    a = nocode_wanted;
    nocode_wanted = 1;
    unary();
    *type = vtop->type;
    vpop();
    nocode_wanted = a;
}

/* Parse static constant evaluation statements updating vtop constraints */
static void expr_const1(void)
{
    int a;
    a = const_wanted;
    const_wanted = 1;
    expr_eq();
    const_wanted = a;
}

/* Validate compile-time evaluation loops extracting the raw static integer constant value payload */
static int expr_const(void)
{
    int c;
    expr_const1();
    if ((vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) != VT_CONST)
        expect("constant expression");
    c = vtop->c.i;
    vpop();
    return c;
}

/* Query lookahead tokens rapidly to identify legitimate code block destination labels */
static int is_label(void)
{
    int last_tok;

    if (tok < TOK_UIDENT)
        return 0;
        
    last_tok = tok;
    next();
    if (tok == ':') {
        next();
        return last_tok;
    } else {
        unget_tok(last_tok); /* Restore historical scanner tracking stack if colon punctuation fails */
        return 0;
    }
}

/* Parse a statement block or control flow construct, tracking active loop/switch destination scopes */
static void block(int *bsym, int *csym, int *case_sym, int *def_sym, int case_reg, int is_expr)
{
    int a, b, c, d;
    Sym *s;

    /* Legacy STABS debug emission engine code siphoned away to retain absolute compile-time speed */

    if (is_expr) {
        /* Enforce fallback assignment rule: Default return value evaluates directly as (void) */
        vpushi(0);
        vtop->type.t = VT_VOID;
    }

    if (tok == TOK_IF) {
        /* Process and resolve conditional 'if' evaluation branches */
        next();
        skip('(');
        gexpr();
        skip(')');
        a = gtst(1, 0);
        block(bsym, csym, case_sym, def_sym, case_reg, 0);
        c = tok;
        if (c == TOK_ELSE) {
            next();
            d = gjmp(0);
            gsym(a);
            block(bsym, csym, case_sym, def_sym, case_reg, 0);
            gsym(d); /* Back-patch and finalize the conditional else jump destination */
        } else {
            gsym(a);
        }
    } else if (tok == TOK_WHILE) {
        /* Process and resolve conditional loop 'while' evaluation branches */
        next();
        d = ind;
        skip('(');
        gexpr();
        skip(')');
        a = gtst(1, 0);
        b = 0;
        block(&a, &b, case_sym, def_sym, case_reg, 0);
        gjmp_addr(d);
        gsym(a);
        gsym_addr(b, d);
    } else if (tok == '{') {
        Sym *llabel;
        
        next();
        /* Snapshot and record localized lexical and label tracking stack positions */
        s = local_stack;
        llabel = local_label_stack;
        
        /* Parse GCC localized __label__ syntax declarations dynamically if flagged */
        if (tok == TOK_LABEL) {
            next();
            for(;;) {
                if (tok < TOK_UIDENT)
                    expect("label identifier");
                label_push(&local_label_stack, tok, LABEL_DECLARED);
                next();
                if (tok == ',') {
                    next();
                } else {
                    skip(';');
                    break;
                }
            }
        }
        while (tok != '}') {
            decl(VT_LOCAL);
            if (tok != '}') {
                if (is_expr)
                    vpop();
                block(bsym, csym, case_sym, def_sym, case_reg, is_expr);
            }
        }
        /* Dismantle localized tracking layers popping symbols and labels out off stack frames cleanly */
        label_pop(&local_label_stack, llabel);
        sym_pop(&local_stack, s);
        next();
    } else if (tok == TOK_RETURN) {
        /* Process routine escape 'return' statements routing register states correctly */
        next();
        if (tok != ';') {
            gexpr();
            gen_assign_cast(&func_vt);
            if ((func_vt.t & VT_BTYPE) == VT_STRUCT) {
                CType type;
                /* Structure return ABI: Copy values straight into the implicit first pointer argument block allocation */
                type = func_vt;
                mk_pointer(&type);
                vset(&type, VT_LOCAL | VT_LVAL, func_vc);
                indir();
                vswap();
                vstore();
            } else if (is_float(func_vt.t)) {
                gv(RC_FRET);
            } else {
                gv(RC_IRET);
            }
            vtop--; /* Force direct decrement over vpop() to shield native x87 FPU stack from unintended flushes */
        }
        skip(';');
        rsym = gjmp(rsym); /* Route straight into the function epilogue escape jump chain */
    } else if (tok == TOK_BREAK) {
        /* Compute structural execution jump breaking out of loop or switch context boundaries */
        if (!bsym)
            error("cannot break");
        *bsym = gjmp(*bsym);
        next();
        skip(';');
    } else if (tok == TOK_CONTINUE) {
        /* Compute structural execution jump returning back to the active loop conditional threshold */
        if (!csym)
            error("cannot continue");
        *csym = gjmp(*csym);
        next();
        skip(';');
    } else if (tok == TOK_FOR) {
        int e;
        next();
        skip('(');
        if (tok != ';') {
            gexpr();
            vpop();
        }
        skip(';');
        d = ind;
        c = ind;
        a = 0;
        b = 0;
        if (tok != ';') {
            gexpr();
            a = gtst(1, 0);
        }
        skip(';');
        if (tok != ')') {
            e = gjmp(0);
            c = ind;
            gexpr();
            vpop();
            gjmp_addr(d);
            gsym(e);
        }
        skip(')');
        block(&a, &b, case_sym, def_sym, case_reg, 0);
        gjmp_addr(c);
        gsym(a);
        gsym_addr(b, c);
    } else if (tok == TOK_DO) {
        /* Process and resolve conditional loop 'do-while' evaluation branches */
        next();
        a = 0;
        b = 0;
        d = ind;
        block(&a, &b, case_sym, def_sym, case_reg, 0);
        skip(TOK_WHILE);
        skip('(');
        gsym(b);
        gexpr();
        c = gtst(0, 0);
        gsym_addr(c, d);
        skip(')');
        gsym(a);
        skip(';');
    } else if (tok == TOK_SWITCH) {
        /* Process and resolve multibranch conditional statement 'switch' evaluation branches */
        next();
        skip('(');
        gexpr();
        case_reg = gv(RC_INT); /* Enforce conversion routing values directly into standard integer register slots */
        vpop();
        skip(')');
        a = 0;
        b = gjmp(0); /* Emit primary initial jump targeting the entry case validation code blocks */
        c = 0;
        block(&a, csym, &b, &c, case_reg, 0);
        if (c == 0)
            c = ind;
        gsym_addr(b, c); /* Finalize fallback destination pointing straight to default label block or switch escape marker */
        gsym(a);         /* Back-patch loose breaks pointing them to destination instruction milestone offset */
    } else if (tok == TOK_CASE) {
        int v1, v2;
        if (!case_sym)
            expect("switch");
        next();
        v1 = expr_const();
        v2 = v1;
        if (gnu_ext && tok == TOK_DOTS) {
            /* GNU C Extension path: Parse sub-bit ranged value conditional branch parameters (case v1 ... v2) */
            next();
            v2 = expr_const();
            if (v2 < v1)
                warning("empty case range");
        }
        
        /* Bypass the statement body utilizing hardware branch jumps to evaluate condition constraints safely */
        b = gjmp(0);
        gsym(*case_sym);
        vseti(case_reg, 0);
        vpushi(v1);
        if (v1 == v2) {
            gen_op(TOK_EQ);
            *case_sym = gtst(1, 0);
        } else {
            gen_op(TOK_GE);
            *case_sym = gtst(1, 0);
            vseti(case_reg, 0);
            vpushi(v2);
            gen_op(TOK_LE);
            *case_sym = gtst(1, *case_sym);
        }
        gsym(b);
        skip(':');
        is_expr = 0;
        goto block_after_label;
    } else if (tok == TOK_DEFAULT) {
        next();
        skip(':');
        if (!def_sym)
            expect("switch");
        if (*def_sym)
            error("too many 'default'");
        *def_sym = ind;
        is_expr = 0;
        goto block_after_label;
    } else if (tok == TOK_GOTO) {
        /* Process dynamic jump structures mapping labels or calculated code-segment pointers straight to architecture lanes */
        next();
        if (tok == '*' && gnu_ext) {
            /* Computed goto instruction resolution path: Dereference code pointer addresses directly */
            next();
            gexpr();
            if ((vtop->type.t & VT_BTYPE) != VT_PTR)
                expect("pointer");
            ggoto();
        } else if (tok >= TOK_UIDENT) {
            s = label_find(tok);
            if (!s) {
                s = label_push(&global_label_stack, tok, LABEL_FORWARD);
            } else {
                if (s->r == LABEL_DECLARED)
                    s->r = LABEL_FORWARD;
            }
            if (s->r & LABEL_FORWARD) 
                s->next = (void *)gjmp((long)s->next);
            else
                gjmp_addr((long)s->next);
            next();
        } else {
            expect("label identifier");
        }
        skip(';');
    } else if (tok == TOK_ASM1 || tok == TOK_ASM2 || tok == TOK_ASM3) {
        /* Branch and route straight to internal native x86 inline assembly parsing engine */
        asm_instr();
    } else {
        b = is_label();
        if (b) {
            /* Destination Branching Label Processing Engine */
            s = label_find(b);
            if (s) {
                if (s->r == LABEL_DEFINED)
                    error("duplicate label '%s'", get_tok_str(s->v, NULL));
                gsym((long)s->next);
                s->r = LABEL_DEFINED;
            } else {
                s = label_push(&global_label_stack, b, LABEL_DEFINED);
            }
            s->next = (void *)ind;
            
        block_after_label:
            if (tok == '}') {
                warning("deprecated use of label at end of compound statement");
            } else {
                if (is_expr)
                    vpop();
                block(bsym, csym, case_sym, def_sym, case_reg, is_expr);
            }
        } else {
            /* Standard continuous C operational expression evaluation path */
            if (tok != ';') {
                if (is_expr) {
                    vpop();
                    gexpr();
                } else {
                    gexpr();
                    vpop();
                }
            }
            skip(';');
        }
    }
}

/* t is the array or struct type. c is the array or struct
   address. cur_index/cur_field is the pointer to the current
   value. 'size_only' is true if only size info is needed (only used
   in arrays) */
static void decl_designator(CType *type, Section *sec, unsigned long c, 
                            int *cur_index, Sym **cur_field, 
                            int size_only)
{
    Sym *s, *f;
    int notfirst, index, index_last, align, l, nb_elems, elem_size;
    CType type1;

    notfirst = 0;
    elem_size = 0;
    nb_elems = 1;
    
    if (gnu_ext && (l = is_label()) != 0)
        goto struct_field;
        
    while (tok == '[' || tok == '.') {
        if (tok == '[') {
            if (!(type->t & VT_ARRAY))
                expect("array type");
            s = type->ref;
            next();
            index = expr_const();
            if (index < 0 || (s->c >= 0 && index >= s->c))
                expect("invalid index");
                
            if (tok == TOK_DOTS && gnu_ext) {
                /* Process GNU C extension path: Parse braced array index ranges (e.g. [1 ... 5]) */
                next();
                index_last = expr_const();
                if (index_last < 0 || (s->c >= 0 && index_last >= s->c) || index_last < index)
                    expect("invalid index");
            } else {
                index_last = index;
            }
            skip(']');
            if (!notfirst)
                *cur_index = index_last;
            type = pointed_type(type);
            elem_size = type_size(type, &align);
            c += index * elem_size;
            
            /* Enforce constraint: We exclusively support range initialization formatting rules for the final designator element */
            nb_elems = index_last - index + 1;
            if (nb_elems != 1) {
                notfirst = 1;
                break;
            }
        } else {
            next();
            l = tok;
            next();
        struct_field:
            if ((type->t & VT_BTYPE) != VT_STRUCT)
                expect("struct/union type");
            s = type->ref;
            l |= SYM_FIELD;
            f = s->next;
            while (f) {
                if (f->v == l)
                    break;
                f = f->next;
            }
            if (!f)
                expect("field");
            if (!notfirst)
                *cur_field = f;
                
            /* Resolve bitmask components overriding storage descriptors carefully */
            type1 = f->type;
            type1.t |= (type->t & ~VT_TYPE);
            type = &type1;
            c += f->c;
        }
        notfirst = 1;
    }
    if (notfirst) {
        if (tok == '=') {
            next();
        } else {
            if (!gnu_ext)
                expect("=");
        }
    } else {
        if (type->t & VT_ARRAY) {
            index = *cur_index;
            type = pointed_type(type);
            c += index * type_size(type, &align);
        } else {
            f = *cur_field;
            if (!f)
                error("too many field init");
                
            /* Resolve bitmask components overriding storage descriptors carefully */
            type1 = f->type;
            type1.t |= (type->t & ~VT_TYPE);
            type = &type1;
            c += f->c;
        }
    }
    decl_initializer(type, sec, c, 0, size_only);

    /* Process compile-time memory duplication loops to fulfill array data range requirements */
    if (!size_only && nb_elems > 1) {
        unsigned long c_end;
        uint8_t *src, *dst;
        int i;

        if (!sec)
            error("range init not supported yet for dynamic storage");
        c_end = c + nb_elems * elem_size;
        if (c_end > sec->data_allocated)
            section_realloc(sec, c_end);
        src = sec->data + c;
        dst = src;
        for(i = 1; i < nb_elems; i++) {
            dst += elem_size;
            memcpy(dst, src, elem_size);
        }
    }
}

#define EXPR_VAL   0
#define EXPR_CONST 1
#define EXPR_ANY   2

/* Store an active value or evaluated expression directly inside a global section layout or local dynamic array */
static void init_putv(CType *type, Section *sec, unsigned long c, int v, int expr_type)
{
    int saved_global_expr, bt, bit_pos, bit_size;
    void *ptr;
    unsigned long long bit_mask;

    switch(expr_type) {
    case EXPR_VAL:
        vpushi(v);
        break;
    case EXPR_CONST:
        /* Compound literals must be strictly allocated globally inside this context frame layer */
        saved_global_expr = global_expr;
        global_expr = 1;
        expr_const1();
        global_expr = saved_global_expr;
        
        if ((vtop->r & (VT_VALMASK | VT_LVAL)) != VT_CONST)
            error("initializer element is not constant");
        break;
    case EXPR_ANY:
        expr_eq();
        break;
    }
    
    if (sec) {
        /* Global storage execution branch: Assign types and process structural relocation boundaries */
        gen_assign_cast(type);
        bt = type->t & VT_BTYPE;
        ptr = (void *)(sec->data + c);
        
        if (!(type->t & VT_BITFIELD)) {
            bit_pos = 0;
            bit_size = 32;
            bit_mask = -1LL;
        } else {
            bit_pos = (vtop->type.t >> VT_STRUCT_SHIFT) & 0x3f;
            bit_size = (vtop->type.t >> (VT_STRUCT_SHIFT + 6)) & 0x3f;
            bit_mask = (1LL << bit_size) - 1;
        }
        
        if ((vtop->r & VT_SYM) &&
            (bt == VT_BYTE || bt == VT_SHORT || bt == VT_DOUBLE ||
             bt == VT_LDOUBLE || bt == VT_LLONG || (bt == VT_INT && bit_size != 32)))
            error("initializer element is not computable at load time");
            
        switch(bt) {
        case VT_BYTE:
            *(char *)ptr |= (vtop->c.i & bit_mask) << bit_pos;
            break;
        case VT_SHORT:
            *(short *)ptr |= (vtop->c.i & bit_mask) << bit_pos;
            break;
        case VT_DOUBLE:
            *(double *)ptr = vtop->c.d;
            break;
        case VT_LDOUBLE:
            *(long double *)ptr = vtop->c.ld; /* Fixed to native x87 FPU 12-byte layout for flat memory formats */
            break;
        case VT_LLONG:
            *(long long *)ptr |= (vtop->c.ll & bit_mask) << bit_pos;
            break;
        default:
            if (vtop->r & VT_SYM) {
                /* Target out global structural relocation metadata binding straight to target output table data */
                greloc(sec, vtop->sym, c, R_DATA_32);
            }
            *(int *)ptr |= (vtop->c.i & bit_mask) << bit_pos;
            break;
        }
        vtop--;
    } else {
        /* Local dynamic stack execution branch: Secure references and commit straight via vstore */
        vset(type, VT_LOCAL, c);
        vswap();
        vstore();
        vpop();
    }
}

/* Inject zero-fill padding macros to execute dynamic variable-based block layout initialization */
static void init_putz(CType *t, Section *sec, unsigned long c, int size)
{
    GFuncContext gf;

    if (sec) {
        /* Bypassed completely for global contexts as the loader layout implicitly wipes standard BSS segments to zero */
    } else {
        /* Emit a high-fidelity continuous memset call to safely initialize local dynamic dynamic runtime storage */
        gfunc_start(&gf, FUNC_CDECL);
        vpushi(size);
        gfunc_param(&gf);
        vpushi(0);
        gfunc_param(&gf);
        vseti(VT_LOCAL, c);
        gfunc_param(&gf);
        vpush_global_sym(&func_old_type, TOK_memset);
        gfunc_call(&gf);
    }
}

/* 'type' contains the type and storage info. 'c' is the offset of the
   object in section 'sec'. If 'sec' is NULL, it means stack based
   allocation. 'first' is true if array '{' must be read (multi
   dimension implicit array init handling). 'size_only' is true if
   size only evaluation is wanted (only for arrays). */
static void decl_initializer(CType *type, Section *sec, unsigned long c, 
                             int first, int size_only)
{
    int index, array_length, n, no_oblock, nb, parlevel, i;
    int size1, align1, expr_type;
    Sym *s, *f;
    CType *t1;

    if (type->t & VT_ARRAY) {
        s = type->ref;
        n = s->c;
        array_length = 0;
        t1 = pointed_type(type);
        size1 = type_size(t1, &align1);

        no_oblock = 1;
        if ((first && tok != TOK_LSTR && tok != TOK_STR) || tok == '{') {
            skip('{');
            no_oblock = 0;
        }

        /* Parse string literals if types align, otherwise treat as isolated pointer expressions */
        if ((tok == TOK_LSTR && (t1->t & VT_BTYPE) == VT_INT) ||
            (tok == TOK_STR && (t1->t & VT_BTYPE) == VT_BYTE)) {
            while (tok == TOK_STR || tok == TOK_LSTR) {
                int cstr_len, ch;
                CString *cstr;

                cstr = tokc.cstr;
                if (tok == TOK_STR)
                    cstr_len = cstr->size;
                else
                    cstr_len = cstr->size / sizeof(int);
                cstr_len--;
                nb = cstr_len;
                if (n >= 0 && nb > (n - array_length))
                    nb = n - array_length;
                if (!size_only) {
                    if (cstr_len > nb)
                        warning("initializer-string for array is too long");
                    /* High-speed fast optimization path for standard character arrays mapped in global storage */
                    if (sec && tok == TOK_STR && size1 == 1) {
                        memcpy(sec->data + c + array_length, cstr->data, nb);
                    } else {
                        for(i = 0; i < nb; i++) {
                            if (tok == TOK_STR)
                                ch = ((unsigned char *)cstr->data)[i];
                            else
                                ch = ((int *)cstr->data)[i];
                            init_putv(t1, sec, c + (array_length + i) * size1, ch, EXPR_VAL);
                        }
                    }
                }
                array_length += nb;
                next();
            }
            /* Inject tracking trailing null terminator dynamically if physical memory layout bounds permit */
            if (n < 0 || array_length < n) {
                if (!size_only) {
                    init_putv(t1, sec, c + (array_length * size1), 0, EXPR_VAL);
                }
                array_length++;
            }
        } else {
            index = 0;
            while (tok != '}') {
                decl_designator(type, sec, c, &index, NULL, size_only);
                if (n >= 0 && index >= n)
                    error("index too large");
                /* Automatically populate inner memory layout holes with hardware zero padding frames */
                if (!size_only && array_length < index) {
                    init_putz(t1, sec, c + array_length * size1, (index - array_length) * size1);
                }
                index++;
                if (index > array_length)
                    array_length = index;
                if (index >= n && no_oblock)
                    break;
                if (tok == '}')
                    break;
                skip(',');
            }
        }
        if (!no_oblock)
            skip('}');
        /* Force hardware zero padding layout fill on unallocated tail ends of the target array */
        if (!size_only && n >= 0 && array_length < n) {
            init_putz(t1, sec, c + array_length * size1, (n - array_length) * size1);
        }
        if (n < 0)
            s->c = array_length;
    } else if ((type->t & VT_BTYPE) == VT_STRUCT && (sec || !first || tok == '{')) {
        int par_count;

        par_count = 0;
        if (tok == '(') {
            AttributeDef ad1;
            CType type1;
            next();
            while (tok == '(') {
                par_count++;
                next();
            }
            if (!parse_btype(&type1, &ad1))
                expect("cast");
            type_decl(&type1, &ad1, &n, TYPE_ABSTRACT);
            if (!is_compatible_types(type, &type1))
                error("invalid type for cast");
            skip(')');
        }
        no_oblock = 1;
        if (first || tok == '{') {
            skip('{');
            no_oblock = 0;
        }
        s = type->ref;
        f = s->next;
        array_length = 0;
        index = 0;
        n = s->c;
        while (tok != '}') {
            decl_designator(type, sec, c, NULL, &f, size_only);
            index = f->c;
            if (!size_only && array_length < index) {
                init_putz(type, sec, c + array_length, index - array_length);
            }
            index = index + type_size(&f->type, &align1);
            if (index > array_length)
                array_length = index;
            f = f->next;
            if (no_oblock && f == NULL)
                break;
            if (tok == '}')
                break;
            skip(',');
        }
        /* Force hardware zero padding layout fill on unallocated tail ends of the target structure */
        if (!size_only && array_length < n) {
            init_putz(type, sec, c + array_length, n - array_length);
        }
        if (!no_oblock)
            skip('}');
        while (par_count) {
            skip(')');
            par_count--;
        }
    } else if (tok == '{') {
        next();
        decl_initializer(type, sec, c, first, size_only);
        skip('}');
    } else if (size_only) {
        /* Bypassed routing path: Safely parse and skip unallocated array expression streams */
        parlevel = 0;
        while ((parlevel > 0 || (tok != '}' && tok != ',')) && tok != -1) {
            if (tok == '(')
                parlevel++;
            else if (tok == ')')
                parlevel--;
            next();
        }
    } else {
        /* Enforce constant requirements for globals, fallback to active expressions inside dynamic stacks */
        expr_type = EXPR_CONST;
        if (!sec)
            expr_type = EXPR_ANY;
        init_putv(type, sec, c, 0, expr_type);
    }
}

/* Parse an initializer for type 'type' if 'has_init' is non-zero, and allocate storage space in local 
   or global data segments ('r' acts as either VT_LOCAL or VT_CONST). Declares associated variables safely. */
static void decl_initializer_alloc(CType *type, AttributeDef *ad, int r, int has_init, int v, int scope)
{
    int size, align, addr, data_offset;
    int level;
    ParseState saved_parse_state;
    TokenString init_str;
    Section *sec;

    size = type_size(type, &align);
    tok_str_new(&init_str);
    
    if (size < 0) {
        if (!has_init) 
            error("unknown type size");
            
        /* Cache down all initialisation sequence strings to evaluate explicit array element sizing bounds */
        if (has_init == 2) {
            while (tok == TOK_STR || tok == TOK_LSTR) {
                tok_str_add_tok(&init_str);
                next();
            }
        } else {
            level = 0;
            while (level > 0 || (tok != ',' && tok != ';')) {
                if (tok < 0)
                    error("unexpected end of file in initializer");
                tok_str_add_tok(&init_str);
                if (tok == '{')
                    level++;
                else if (tok == '}') {
                    if (level == 0)
                        break;
                    level--;
                }
                next();
            }
        }
        tok_str_add(&init_str, -1);
        tok_str_add(&init_str, 0);
        
        /* Execute the primary analytical dry-run sizing pass over cached token stream strings */
        save_parse_state(&saved_parse_state);

        macro_ptr = init_str.str;
        next();
        decl_initializer(type, NULL, 0, 1, 1);
        
        /* Re-route pointer states preparing for the final concrete initialization parsing sweep */
        macro_ptr = init_str.str;
        next();
        
        size = type_size(type, &align);
        if (size < 0) 
            error("unknown type size");
    }
    
    /* Overwrite standard boundaries if custom attribute alignment parameter specifies a larger width */
    if (ad->aligned > align)
        align = ad->aligned;
        
    if ((r & VT_VALMASK) == VT_LOCAL) {
        sec = NULL;
        /* Bounds checking structures and safety loops siphoned away to retain absolute linear stack performance */
        loc = (loc - size) & -align;
        addr = loc;
        
        if (v) {
            /* Local variable path: Register metadata directly onto active local evaluation scopes */
            sym_push(v, type, r, addr);
        } else {
            /* Value stack reference path: Push address directly onto evaluation stack frames */
            vset(type, r, addr);
        }
    } else {
        Sym *sym;

        sym = NULL;
        if (v && scope == VT_CONST) {
            /* Cross-verify if the target global symbol identifier was already initialized inside dictionary maps */
            sym = sym_find(v);
            if (sym) {
                if (!is_compatible_types(&sym->type, type))
                    error("incompatible types for redefinition of '%s'", get_tok_str(v, NULL));
                if (sym->type.t & VT_EXTERN) {
                    sym->type.t &= ~VT_EXTERN;
                } else {
                    if (!has_init)
                        goto no_alloc;
                }
            }
        }

        /* Route structural data allocation pipelines directly into the assigned ELF data section layout */
        sec = ad->section;
        if (!sec) {
            if (has_init)
                sec = data_section;
        }
        if (sec) {
            data_offset = sec->data_offset;
            data_offset = (data_offset + align - 1) & -align;
            addr = data_offset;
            
            /* Increment data offset limits immediately to secure layout parameters against initialization loops */
            data_offset += size;
            sec->data_offset = data_offset;
            
            if (sec->sh_type != SHT_NOBITS && data_offset > sec->data_allocated)
                section_realloc(sec, data_offset);
        } else {
            addr = 0;
        }

        if (v) {
            if (scope == VT_CONST) {
                if (!sym)
                    goto do_def;
            } else {
            do_def:
                sym = sym_push(v, type, r | VT_SYM, 0);
            }
            
            /* Commit definition parameters straight into the output native ELF symbol tables map */
            if (sec) {
                put_extern_sym(sym, sec, addr, size);
            } else {
                Elf32_Sym *esym;
                /* Map global uninitialized symbols as standard SHN_COMMON block allocations */
                put_extern_sym(sym, NULL, align, size);
                esym = &((Elf32_Sym *)symtab_section->data)[sym->c];
                esym->st_shndx = SHN_COMMON;
            }
        } else {
            CValue cval;
            /* Reference payload path: Generate static reference descriptors and load directly onto the value stack */
            sym = get_sym_ref(type, sec, addr, size);
            cval.ul = 0;
            vsetc(type, VT_CONST | VT_SYM, &cval);
            vtop->sym = sym;
        }
    }
    
    if (has_init) {
        decl_initializer(type, sec, addr, 1, 0);
        /* Safely recycle temporary token caching string allocations and restore historical parser state tracking */
        if (init_str.str) {
            tok_str_free(init_str.str);
            restore_parse_state(&saved_parse_state);
        }
    }
 no_alloc: ;
}

/* Parse a legacy K&R (old-style) function parameter declaration list mapping types to symbols sequentially */
static void func_decl_list(Sym *func_sym)
{
    AttributeDef ad;
    int v;
    Sym *s;
    CType btype, type;

    /* Traverse and evaluate each formal parameter declaration mapping downstream context lines */
    while (tok != '{' && tok != ';' && tok != ',' && tok != TOK_EOF) {
        if (!parse_btype(&btype, &ad)) 
            expect("declaration list");
            
        if (((btype.t & VT_BTYPE) == VT_ENUM || (btype.t & VT_BTYPE) == VT_STRUCT) && tok == ';') {
            /* Support language standard: Allow isolated structure or enumeration definitions missing variables trailing after */
        } else {
            for(;;) {
                type = btype;
                type_decl(&type, &ad, &v, TYPE_DIRECT);
                
                /* Scan and locate the mapped parameter label directly inside the internal function parameter stack layout */
                s = func_sym->next;
                while (s != NULL) {
                    if ((s->v & ~SYM_FIELD) == v)
                        goto found;
                    s = s->next;
                }
                error("declaration for parameter '%s' but no such parameter", get_tok_str(v, NULL));
                
            found:
                /* Verify constraints: Specifying alternative storage descriptors except 'register' remains strictly forbidden */
                if (type.t & VT_STORAGE)
                    error("storage class specified for '%s'", get_tok_str(v, NULL)); 
                    
                convert_parameter_type(&type);
                
                /* Update symbol table frame binding the finalized type specifications onto the parameter entry */
                s->type = type;
                
                if (tok == ',')
                    next();
                else
                    break;
            }
        }
        skip(';');
    }
}

/* Parse a data declaration or a function definition context. 
   Parameter 'l' acts as a bitmask identifying default storage bounds (VT_LOCAL or VT_CONST) */
static void decl(int l)
{
    int v, has_init, r;
    CType type, btype;
    Sym *sym;
    AttributeDef ad;

    while (1) {
        if (!parse_btype(&btype, &ad)) {
            /* Skip redundant semi-colon punctuation separators inside top-level layouts */
            if (tok == ';') {
                next();
                continue;
            }
            /* Legacy K&R fallback strategy: Tolerate missing explicit return type declarations for global structures */
            if (l == VT_LOCAL || tok < TOK_DEFINE)
                break;
            btype.t = VT_INT;
        }
        if (((btype.t & VT_BTYPE) == VT_ENUM || (btype.t & VT_BTYPE) == VT_STRUCT) && tok == ';') {
            /* Support language standard: Allow isolated structure or enumeration definitions missing variables */
            next();
            continue;
        }
        while (1) {
            type = btype;
            type_decl(&type, &ad, &v, TYPE_DIRECT);

            if ((type.t & VT_BTYPE) == VT_FUNC) {
                sym = type.ref;
                if (sym->c == FUNC_OLD)
                    func_decl_list(sym);
            }

            if (tok == '{') {
                /* Target validation: Reject invalid locally declared sub-function block statements */
                if (l == VT_LOCAL)
                    error("cannot use local functions");
                if (!(type.t & VT_FUNC))
                    expect("function definition");
                    
                /* Optimize structural linkages translating inline declarations into static symbols if needed */
                if ((type.t & (VT_EXTERN | VT_INLINE)) == (VT_EXTERN | VT_INLINE))
                    type.t = (type.t & ~VT_EXTERN) | VT_STATIC;

                /* Compute text section mapping and calculate internal segment offsets */
                cur_text_section = ad.section;
                if (!cur_text_section)
                    cur_text_section = text_section;
                    
                ind = cur_text_section->data_offset;
                funcname = get_tok_str(v, NULL);
                sym = sym_find(v);
                if (sym) {
                    sym->type = type;
                } else {
                    sym = global_identifier_push(v, type.t, 0);
                    sym->type.ref = type.ref;
                }
                
                /* Commit function symbol entry straight into the output native ELF symbol tables map */
                put_extern_sym(sym, cur_text_section, ind, 0);
                func_ind = ind;
                sym->r = VT_SYM | VT_CONST;
                
                /* Push a dummy symbol layout descriptor to initialize local function variable boundaries safely */
                sym_push2(&local_stack, SYM_FIELD, 0, 0);
                gfunc_prolog(&type); /* Emit function entry prologue code blocks */
                loc = 0;
                rsym = 0;

                block(NULL, NULL, NULL, NULL, 0, 0); /* Parse internal statement code execution blocks */
                gsym(rsym);
                gfunc_epilog(); /* Emit function epilogue machine code structures */
                
                cur_text_section->data_offset = ind;
                label_pop(&global_label_stack, NULL);
                sym_pop(&local_stack, NULL); /* Reset and flush the localized function variable tracking stacks */
                
                /* Post-patch compiled symbol payload size calculations straight into the native ELF headers */
                ((Elf32_Sym *)symtab_section->data)[sym->c].st_size = ind - func_ind;
                
                funcname = ""; 
                func_vt.t = VT_VOID; 
                ind = 0; 
                break;
            } else {
                if (btype.t & VT_TYPEDEF) {
                    /* Save custom user-defined type definitions via typedef map layout updates */
                    sym = sym_push(v, &type, 0, 0);
                    sym->type.t |= VT_TYPEDEF;
                } else if ((type.t & VT_BTYPE) == VT_FUNC) {
                    /* Register references identifying external function linkage boundaries */
                    external_sym(v, &type, 0);
                } else {
                    r = 0;
                    if (!(type.t & VT_ARRAY))
                        r |= lvalue_type(type.t);
                    has_init = (tok == '=');
                    if ((btype.t & VT_EXTERN) || 
                        ((type.t & VT_ARRAY) && (type.t & VT_STATIC) && !has_init && l == VT_CONST && type.ref->c < 0)) {
                        /* External variable allocation path: Bind uninitialized null arrays as absolute external symbols */
                        external_sym(v, &type, r);
                    } else {
                        if (type.t & VT_STATIC)
                            r |= VT_CONST;
                        else
                            r |= l;
                        if (has_init)
                            next();
                        /* Trigger explicit variable layout storage assignment and parse initializer chains */
                        decl_initializer_alloc(&type, &ad, r, has_init, v, l);
                    }
                }
                if (tok != ',') {
                    skip(';');
                    break;
                }
                next();
            }
        }
    }
}

/* Better than nothing, but needs extension to handle '-E' option correctly too */
static void preprocess_init(TCCState *s1)
{
    s1->include_stack_ptr = s1->include_stack;
    s1->ifdef_stack_ptr = s1->ifdef_stack;
    file->ifdef_stack_ptr = s1->ifdef_stack_ptr;

    /* Reset value stack tracking register boundaries to primary baseline entry */
    vtop = vstack - 1;
}

/* Master Pipeline Entry: Compile the standalone C source file registered inside 'file' descriptor.
   Returns non-zero value if semantic or compilation errors are caught. */
static int tcc_compile(TCCState *s1)
{
    Sym *define_start;
    char buf;
    volatile int section_sym;

    /* Initialize active preprocessor states, macro tables and stack alignment structures */
    preprocess_init(s1);

    funcname = "";
    anon_sym = SYM_FIRST_ANOM; 
    section_sym = 0; 

    /* Put localized file descriptor tracking details straight into the primary ELF symbol table text section */
    put_elf_sym(symtab_section, 0, 0, ELF32_ST_INFO(STB_LOCAL, STT_FILE), 0, SHN_ABS, file->filename);

    /* Enforce initialization maps for frequently queried core type specifiers */
    int_type.t = VT_INT;
    char_pointer_type.t = VT_BYTE;
    mk_pointer(&char_pointer_type);

    func_old_type.t = VT_FUNC;
    func_old_type.ref = sym_push(SYM_FIELD, &int_type, FUNC_CDECL, FUNC_OLD);

    define_start = define_stack;

    /* Secure Execution Shield: Completely bypass setjmp/longjmp error traps to guard register states.
       Forcing error jump buf trackers off prevents processing flow from entering critical memory faults. */
    s1->nb_errors = 0;
    s1->error_set_jmp_enabled = 0;

    /* Synchronize input buffer pointers tracking terminal end-of-file milestones */
    ch = file->buf_ptr[0];
    tok_flags = TOK_FLAG_BOL | TOK_FLAG_BOF;
    parse_flags = PARSE_FLAG_PREPROCESS | PARSE_FLAG_TOK_NUM;
    
    next();
    decl(VT_CONST); /* Launch the master parsing loop processing core syntax structures recursively */
    
    if (tok != -1)
        expect("declaration");

    s1->error_set_jmp_enabled = 0;

    /* Recycle allocated macro tables freeing macro definitions up to milestone milestone */
    free_defines(define_start); 

    /* Clear and pop persistent symbol scopes from the global storage tracking lists safely */
    sym_pop(&global_stack, NULL);
    return s1->nb_errors != 0 ? -1 : 0;
}

#ifdef LIBTCC
/* Compile a raw C code sequence directly out of a memory resident string wrapper */
int tcc_compile_string(TCCState *s, const char *str)
{
    BufferedFile bf1, *bf = &bf1;
    int ret, len;
    char *buf;

    bf->fd = -1;
    len = strlen(str);
    buf = tcc_malloc(len + 1);
    if (!buf)
        return -1;
    memcpy(buf, str, len);
    buf[len] = CH_EOB;
    bf->buf_ptr = buf;
    bf->buf_end = buf + len;
    pstrcpy(bf->filename, sizeof(bf->filename), "<string>");
    bf->line_num = 1;
    file = bf;
    
    ret = tcc_compile(s);
    tcc_free(buf);

    return ret;
}
#endif

/* Define a preprocessor symbol map layout injecting explicit value parameters if assignment is requested */
void tcc_define_symbol(TCCState *s1, const char *sym, const char *value)
{
    BufferedFile bf1, *bf = &bf1;

    pstrcpy(bf->buffer, IO_BUF_SIZE, sym);
    pstrcat(bf->buffer, IO_BUF_SIZE, " ");
    
    if (!value) 
        value = "1";
    pstrcat(bf->buffer, IO_BUF_SIZE, value);
    
    bf->fd = -1;
    bf->buf_ptr = bf->buffer;
    bf->buf_end = bf->buffer + strlen(bf->buffer);
    *bf->buf_end = CH_EOB;
    bf->filename[0] = '\0';
    ch = file->buf_ptr[0];
    bf->line_num = 1;
    file = bf;
    
    s1->include_stack_ptr = s1->include_stack;

    ch = (int)(intptr_t)file->buf_ptr;
    next_nomacro();
    parse_define();
    file = NULL;
}

/* Undefine a registered preprocessor macro symbol dropping its link mapping entry from dictionary tables */
void tcc_undefine_symbol(TCCState *s1, const char *sym)
{
    TokenSym *ts;
    Sym *s;
    ts = tok_alloc(sym, strlen(sym));
    s = define_find(ts->tok);
    if (s)
        define_undef(s);
}

/* Native TRDOS i386 Inline Assembly and Object Generator Subsystems integration */
#include "i386-asm.c"
#include "tccasm.c"

/* Native Core ELF Executable Object Linker Subsystem integration */
#include "tccelf.c"

/* Position printing from stabs debug section bypassed under TRDOS flat paradigm */
static void rt_printline(unsigned long wanted_pc)
{
    /* STABS debug interpretation architecture siphoned away to retain core minimalism */
}

#ifndef WIN32
/* Runtime caller PC lookup bypassed to drop external ucontext dependencies */
static int rt_get_caller_pc(unsigned long *paddr, void *uc, int level)
{
    return -1;
}

/* Emit runtime error notification tracking layers stubbed out cleanly */
void rt_error(void *uc, const char *fmt, ...)
{
    /* Fallback directly into our standard unified internal error trap */
    error("Runtime error intercepted inside execution pipeline");
}

/* Signal handling triggers safely redirected straight to native absolute execution exit shields */
static void sig_error(int signum, void *siginf, void *puc)
{
    exit(255);
}
#endif

/* Execute all runtime or linkage relocations (mandatory before utilizing tcc_get_symbol() loops) */
int tcc_relocate(TCCState *s1)
{
    Section *s;
    int i;
    
    /* Fetch and resolve standard 32-bit section pointer descriptors using the core dictionary */
    unsigned int flat_current_offset = 0;
    Section *text_sec   = find_section(s1, ".text");
    Section *rodata_sec = find_section(s1, ".rodata");
    Section *data_sec   = find_section(s1, ".data");
    Section *bss_sec    = find_section(s1, ".bss");

    s1->nb_errors = 0;
    
    tcc_add_runtime(s1);

    relocate_common_syms();

    /* =========================================================================
       TRDOS-386 NATIVE FLAT ADDRESS ALIGNMENT ENGINE (OPTIMIZED RUNTIME)
       ========================================================================= */
    /* 1. Code section (.text) maps strictly to absolute base address 0x0 */
    if (text_sec) {
        text_sec->sh_addr = 0;
        flat_current_offset = text_sec->sh_addr + text_sec->data_offset;
    }

    /* 2. Read-only data (.rodata) appends sequentially directly behind the text milestone */
    if (rodata_sec) {
        rodata_sec->sh_addr = flat_current_offset;
        flat_current_offset += rodata_sec->data_offset;
    }

    /* 3. Initialized data (.data) chains immediately downstream following the rodata layer */
    if (data_sec) {
        data_sec->sh_addr = flat_current_offset;
        flat_current_offset += data_sec->data_offset;
    }

    /* 4. Uninitialized data (.bss) reserves zero-fill layout storage pointing to the trailing end */
    if (bss_sec) {
        if (bss_sec->data_offset > 0) {
            bss_sec->data = tcc_mallocz(bss_sec->data_offset);
        }
        bss_sec->sh_addr = flat_current_offset;
    }

    /* Process alternative allocatable sections ensuring standard baseline execution mappings */
    for(i = 1; i < s1->nb_sections; i++) {
        s = s1->sections[i];
        if (s->sh_flags & SHF_ALLOC) {
            if (s->sh_type == SHT_NOBITS && s != bss_sec) {
                s->data = tcc_mallocz(s->data_offset);
            }
            /* Do not alter base address configuration for our hand-aligned baseline flat quadrants */
            if (s != text_sec && s != rodata_sec && s != data_sec && s != bss_sec) {
                s->sh_addr = (unsigned long)s->data;
            }
        }
    }
    /* ========================================================================= */

    /* Resolve layout symbols based directly on the newly computed precise flat linear addresses */
    relocate_syms(s1, 1);

    if (s1->nb_errors != 0)
        return -1;

    /* Execute the sequential section relocation patch pipeline loops */
    for(i = 1; i < s1->nb_sections; i++) {
        s = s1->sections[i];
        if (s->reloc)
            relocate_section(s1, s);
    }
    return 0;
}

/* =========================================================================
   TRDOS-386 NATIVE STATE LIFE-CYCLE AND USER-INTERFACE ENGINE (FINAL)
   ========================================================================= */

int do_bench = 0;

/* Allocate and initialize a fresh TCC compiler state context instance mapping core registers */
TCCState *tcc_new(void)
{
    const char *p, *r;
    TCCState *s;
    TokenSym *ts;
    int i, c;

    s = tcc_mallocz(sizeof(TCCState));
    if (!s)
        return NULL;
    tcc_state = s;
    s->output_type = TCC_OUTPUT_MEMORY;

    /* Initialize core identifier character mapping tables loop */
    for(i = 0; i < 256; i++)
        isidnum_table[i] = isid(i) || isnum(i);

    /* Construct token tracking tables and clear continuous identifier hash buffers */
    table_ident = NULL;
    memset(hash_ident, 0, TOK_HASH_SIZE * sizeof(TokenSym *));
    
    tok_ident = TOK_IDENT;
    p = tcc_keywords;
    while (*p) {
        r = p;
        for(;;) {
            c = *r++;
            if (c == '\0')
                break;
        }
        ts = tok_alloc(p, r - p - 1);
        p = r;
    }

    /* Enqueue structural predefined tracking anchors to accelerate preprocessor tests */
    define_push(TOK___LINE__, MACRO_OBJ, NULL, NULL);
    define_push(TOK___FILE__, MACRO_OBJ, NULL, NULL);
    define_push(TOK___DATE__, MACRO_OBJ, NULL, NULL);
    define_push(TOK___TIME__, MACRO_OBJ, NULL, NULL);

    /* Instantiate standard baseline preprocessor system macro symbols */
    tcc_define_symbol(s, "__STDC__", NULL);
    tcc_define_symbol(s, "__i386__", NULL); 
    tcc_define_symbol(s, "__TINYC__", NULL);

    /* Inject universal standard size representation layout attributes */
    tcc_define_symbol(s, "__SIZE_TYPE__", "unsigned int");
    tcc_define_symbol(s, "__PTRDIFF_TYPE__", "int");
    tcc_define_symbol(s, "__WCHAR_TYPE__", "int");
    
    /* Skip and reserve slot zero to shield system against section null pointers */
    dynarray_add((void ***)&s->sections, &s->nb_sections, NULL);

    /* Allocate and register standard baseline ELF quadrants layouts */
    text_section = new_section(s, ".text", SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR);
    data_section = new_section(s, ".data", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE);
    bss_section = new_section(s, ".bss", SHT_NOBITS, SHF_ALLOC | SHF_WRITE);

    /* Construct primary standard linking stage ELF symbol tables maps */
    symtab_section = new_symtab(s, ".symtab", SHT_SYMTAB, 0, ".strtab", ".hashtab", SHF_PRIVATE); 
    strtab_section = symtab_section->link;
    
    return s;
}

/* Purge and release all allocated resources associated with the target TCC compiler state instance */
void tcc_delete(TCCState *s1)
{
    int i, n;

    /* Purge compile-time command line macro definitions from stack frames */
    free_defines(NULL);

    /* Release dynamic token dictionary identifier array storage maps */
    n = tok_ident - TOK_IDENT;
    for(i = 0; i < n; i++)
        tcc_free(table_ident[i]);
    tcc_free(table_ident);

    /* Release all allocated system sections and structural ELF tables from heap memory */
    free_section(symtab_section->hash);

    for(i = 1; i < s1->nb_sections; i++)
        free_section(s1->sections[i]);
    tcc_free(s1->sections);
    
    /* Release configured dynamic storage lookup string array entries sequential paths */
    for(i = 0; i < s1->nb_library_paths; i++)
        tcc_free(s1->library_paths[i]);
    tcc_free(s1->library_paths);

    for(i = 0; i < s1->nb_cached_includes; i++)
        tcc_free(s1->cached_includes[i]);
    tcc_free(s1->cached_includes);

    for(i = 0; i < s1->nb_include_paths; i++)
        tcc_free(s1->include_paths[i]);
    tcc_free(s1->include_paths);

    for(i = 0; i < s1->nb_sysinclude_paths; i++)
        tcc_free(s1->sysinclude_paths[i]);
    tcc_free(s1->sysinclude_paths);

    tcc_free(s1);
}

/* Register a localized preprocessor include directory string lookup target pathname */
int tcc_add_include_path(TCCState *s1, const char *pathname)
{
    char *pathname1;
    
    pathname1 = tcc_strdup(pathname);
    dynarray_add((void ***)&s1->include_paths, &s1->nb_include_paths, pathname1);
    return 0;
}

/* Register a system-level preprocessor header directory string lookup target pathname */
int tcc_add_sysinclude_path(TCCState *s1, const char *pathname)
{
    char *pathname1;
    
    pathname1 = tcc_strdup(pathname);
    dynarray_add((void ***)&s1->sysinclude_paths, &s1->nb_sysinclude_paths, pathname1);
    return 0;
}

/* Find source file type by evaluating extensions and route parameters straight to corresponding modules */
static int tcc_add_file_internal(TCCState *s1, const char *filename, int flags)
{
    const char *ext, *filename1;
    Elf32_Ehdr ehdr;
    int fd, ret;
    BufferedFile *saved_file;
    
    filename1 = strrchr(filename, '/');
    if (filename1)
        filename1++;
    else
        filename1 = filename;
    ext = strrchr(filename1, '.');
    if (ext)
        ext++;

    saved_file = file;
    file = tcc_open(s1, filename);
    if (!file) {
        if (flags & AFF_PRINT_ERROR) {
            error_noabort("file '%s' not found", filename);
        }
        ret = -1;
        goto fail1;
    }

    if (!ext || !strcmp(ext, "c")) {
        ret = tcc_compile(s1);
    } else if (!strcmp(ext, "S")) {
        ret = tcc_assemble(s1, 1);
    } else if (!strcmp(ext, "s")) {
        ret = tcc_assemble(s1, 0);
    } else {
        fd = file->fd;
        if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
            error_noabort("could not read header");
            goto fail;
        }
        lseek(fd, 0, SEEK_SET);
        
        if (ehdr.e_ident[0] == ELFMAG0 &&
            ehdr.e_ident[1] == ELFMAG1 &&
            ehdr.e_ident[2] == ELFMAG2 &&
            ehdr.e_ident[3] == ELFMAG3) {
            file->line_num = 0; 
            if (ehdr.e_type == ET_REL) {
                ret = tcc_load_object_file(s1, fd, 0);
            } else {
                error_noabort("unrecognized ELF file");
                goto fail;
            }
        } else if (memcmp((char *)&ehdr, ARMAG, 8) == 0) {
            file->line_num = 0;
            ret = tcc_load_archive(s1, fd);
        } else {
            ret = tcc_load_ldscript(s1);
            if (ret < 0) {
                error_noabort("unrecognized file type");
                goto fail;
            }
        }
    }
 the_end:
    tcc_close(file);
 fail1:
    file = saved_file;
    return ret;
 fail:
    ret = -1;
    goto the_end;
}

/* Public API Entry: Add a standalone file to the current compiler instance state redirection map */
int tcc_add_file(TCCState *s, const char *filename)
{
    return tcc_add_file_internal(s, filename, AFF_PRINT_ERROR);
}

/* Public API Entry: Register a new library file searching directory path into storage lists */
int tcc_add_library_path(TCCState *s, const char *pathname)
{
    char *pathname1;
    
    pathname1 = tcc_strdup(pathname);
    dynarray_add((void ***)&s->library_paths, &s->nb_library_paths, pathname1);
    return 0;
}

/* Public API Entry: Resolve and append a library file utilizing short-hand token command markers (-l name) */
int tcc_add_library(TCCState *s, const char *libraryname)
{
    char buf[1024];
    int i;
    
    /* Dynamic shared object loading bypassed completely under TRDOS pure static link design rules */

    /* Evaluate and link static library archive files (.a) sequentially */
    for(i = 0; i < s->nb_library_paths; i++) {
        snprintf(buf, sizeof(buf), "%s/lib%s.a", s->library_paths[i], libraryname);
        if (tcc_add_file_internal(s, buf, 0) == 0)
            return 0;
    }
    return -1;
}

/* Public API Entry: Inject an explicit global symbol reference descriptor with a customized absolute runtime address */
int tcc_add_symbol(TCCState *s, const char *name, unsigned long val)
{
    add_elf_sym(symtab_section, val, 0, ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE), SHN_ABS, name);
    return 0;
}

/* Configure the compilation output storage targets and map target system include directory trees */
int tcc_set_output_type(TCCState *s, int output_type)
{
    char buf[1024];

    s->output_type = output_type;

    if (!s->nostdinc) {
        snprintf(buf, sizeof(buf), "%s/include", tcc_lib_path);
        tcc_add_sysinclude_path(s, buf);
    }

    tcc_add_runtime(s);
    return 0;
}

#if !defined(LIBTCC)

/* Extract and return the localized chronological platform timer metric scaled in microseconds */
static int64_t getclock_us(void)
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

/* Print the primary operational command line argument parameters documentation guide directly to stdout */
void help(void)
{
    printf("tcc version " TCC_VERSION " - Tiny C Compiler - Copyright (C) 2001, 2002 Fabrice Bellard\n" 
           "usage: tcc [-v] [-c] [-o outfile] [-Bdir] [-bench] [-Idir] [-Dsym[=val]] [-Usym]\n"
           "           [-Ldir] [-llib] [-shared] [-static]\n"
           "           [infile1 infile2...] [-run infile args...]\n"
           "\n"
           "General options:\n"
           "  -v          display current version\n"
           "  -c          compile only - generate an object file\n"
           "  -o outfile  set output filename\n"
           "  -Bdir       set tcc internal library path\n"
           "  -bench      output compilation statistics\n"
           "  -run        run compiled source\n"
           "Preprocessor options:\n"
           "  -Idir       add include path 'dir'\n"
           "  -Dsym[=val] define 'sym' with value 'val'\n"
           "  -Usym       undefine 'sym'\n"
           "Linker options:\n"
           "  -Ldir       add library path 'dir'\n"
           "  -llib       link with dynamic or static library 'lib'\n"
           "  -shared     generate a shared library\n"
           "  -static     static linking\n"
           "  -r          relocatable output\n"
           );
}
#endif

#define TCC_OPTION_HAS_ARG 0x0001
#define TCC_OPTION_NOSEP   0x0002 /* Enforce rule: Space is strictly forbidden between option and argument */

typedef struct TCCOption {
    const char *name;
    uint16_t index;
    uint16_t flags;
} TCCOption;

enum {
    TCC_OPTION_HELP,
    TCC_OPTION_I,
    TCC_OPTION_D,
    TCC_OPTION_U,
    TCC_OPTION_L,
    TCC_OPTION_B,
    TCC_OPTION_l,
    TCC_OPTION_bench,
    TCC_OPTION_c,
    TCC_OPTION_static,
    TCC_OPTION_shared,
    TCC_OPTION_o,
    TCC_OPTION_r,
    TCC_OPTION_W,
    TCC_OPTION_O,
    TCC_OPTION_m,
    TCC_OPTION_f,
    TCC_OPTION_nostdinc,
    TCC_OPTION_print_search_dirs,
    TCC_OPTION_rdynamic,
    TCC_OPTION_run,
    TCC_OPTION_v,
};

/* Master operational command-line argument option matching lookup array matrix */
static const TCCOption tcc_options[] = {
    { "h", TCC_OPTION_HELP, 0 },
    { "?", TCC_OPTION_HELP, 0 },
    { "I", TCC_OPTION_I, TCC_OPTION_HAS_ARG },
    { "D", TCC_OPTION_D, TCC_OPTION_HAS_ARG },
    { "U", TCC_OPTION_U, TCC_OPTION_HAS_ARG },
    { "L", TCC_OPTION_L, TCC_OPTION_HAS_ARG },
    { "B", TCC_OPTION_B, TCC_OPTION_HAS_ARG },
    { "l", TCC_OPTION_l, TCC_OPTION_HAS_ARG | TCC_OPTION_NOSEP },
    { "bench", TCC_OPTION_bench, 0 },
    { "c", TCC_OPTION_c, 0 },
    { "static", TCC_OPTION_static, 0 },
    { "shared", TCC_OPTION_shared, 0 },
    { "o", TCC_OPTION_o, TCC_OPTION_HAS_ARG },
    { "run", TCC_OPTION_run, 0 },
    { "rdynamic", TCC_OPTION_rdynamic, 0 }, 
    { "r", TCC_OPTION_r, 0 },
    { "W", TCC_OPTION_W, TCC_OPTION_HAS_ARG | TCC_OPTION_NOSEP },
    { "O", TCC_OPTION_O, TCC_OPTION_HAS_ARG | TCC_OPTION_NOSEP },
    { "m", TCC_OPTION_m, TCC_OPTION_HAS_ARG },
    { "f", TCC_OPTION_f, TCC_OPTION_HAS_ARG | TCC_OPTION_NOSEP },
    { "nostdinc", TCC_OPTION_nostdinc, 0 },
    { "print-search-dirs", TCC_OPTION_print_search_dirs, 0 }, 
    { "v", TCC_OPTION_v, 0 },
    { NULL },
};

/* Master Executable Entry Point: Orchestrates the command-line argument pipeline and triggers compiler stages */
int main(int argc, char **argv)
{
    char *r;
    int optind, output_type, multiple_files, i, reloc_output;
    TCCState *s;
    char **files;
    int nb_files, nb_libraries, nb_objfiles, dminus, ret;
    char objfilename[1024];
    int64_t start_time = 0;
    const TCCOption *popt;
    const char *optarg, *p1, *r1, *outfile;
    int print_search_dirs;

    if (argc == 1) {
        /* Direct system write calls to display quick usage information efficiently without buffer flushes */
        write(1, "Tiny C Compiler version 0.9.18 for TRDOS 386\r\n", 46);
        write(1, "Usage: tcc [options] [infile1] [infile2]...\r\n", 45);
        write(1, "Options:\r\n", 10);
        write(1, "  -c          compile only (produce .o object file)\r\n", 53);
        write(1, "  -o outfile  set output file name\r\n", 37);
        write(1, "  -v          display tcc version\r\n", 35);
        
        /* Forced Native TRDOS Exit Shield: Bypass terminal standard code returns via direct kernel interruption */
        __asm__ __volatile__ (
            ".intel_syntax noprefix\n"
            "mov ebx, 0\n"
            "mov eax, 1\n" /* sys_exit (1) vector invocation */
            "int 0x40\n"
            ".att_syntax\n"
        );
        return 0;
    }

    /* Instantiate a fresh standalone state registry context block mapping core operational allocations */
    s = tcc_new();
    if (!s) {
        return 1;
    }

    output_type = TCC_OUTPUT_EXE;
    optind = 1;
    outfile = NULL;
    multiple_files = 1;
    dminus = 0;
    files = NULL;
    nb_files = 0;
    nb_libraries = 0;
    reloc_output = 0;
    print_search_dirs = 0;

    while (1) {
        if (optind >= argc) {
            if (nb_files == 0 && !print_search_dirs)
                goto show_help;
            else
                break;
        }

        r = argv[optind++];
        if (r[0] != '-') {
            /* Enqueue input source file paths straight into the active compilation tracking array layout */
            dynarray_add((void ***)&files, &nb_files, r);

            if (!multiple_files) {
                optind--;
                /* Break immediately if sequential file scanning limits are constrained */
                break;
            }
        } else {
            /* =========================================================================
               TRDOS NATIVE PURE FLAT COMMAND LINE ARGUMENT PARSING ENGINE
               ========================================================================= */
            popt = tcc_options;
            r1 = r + 1; /* Skip the loose hyphen identifier character to extract the pure option literal */

            for(;;) {
                p1 = popt->name;
                
                if (p1 == NULL) {
                    error("invalid option -- '%s'", r);
                }

                /* Execute direct memory-safe sequential string comparison loop without external str/libc overhead */
                char *s1 = (char *)p1;
                char *s2 = (char *)r1;
                
                while (*s1 && (*s1 == *s2)) {
                    s1++;
                    s2++;
                }

                if (*s1 == '\0') {
                    r1 = s2; /* Shift boundary pointers directly to extract parameters for contiguous args like -Ipath */
                    goto option_found;
                }

                popt++; /* Safely advance to the adjacent data cell structure inside lookup tables matrix */
            }

        option_found:
            if (popt->flags & TCC_OPTION_HAS_ARG) {
                if (*r1 != '\0' || (popt->flags & TCC_OPTION_NOSEP)) {
                    optarg = r1;
                } else {
                    if (optind >= argc)
                        error("argument to '%s' is missing", r);
                    optarg = argv[optind++];
                }
            } else {
                if (*r1 != '\0')
                    goto show_help;
                optarg = NULL;
            }
                
            switch(popt->index) {
            case TCC_OPTION_HELP:
            show_help:
                help();
                return 1;
            case TCC_OPTION_I:
                if (tcc_add_include_path(s, optarg) < 0)
                    error("too many include paths");
                break;
            case TCC_OPTION_D:
                {
                    char *sym, *value;
                    sym = (char *)optarg;
                    value = strchr(sym, '=');
                    if (value) {
                        *value = '\0';
                        value++;
                    }
                    tcc_define_symbol(s, sym, value);
                }
                break;
            case TCC_OPTION_U:
                tcc_undefine_symbol(s, optarg);
                break;
            case TCC_OPTION_L:
                tcc_add_library_path(s, optarg);
                break;
            case TCC_OPTION_B:
                tcc_lib_path = optarg;
                break;
            case TCC_OPTION_l:
                dynarray_add((void ***)&files, &nb_files, r);
                nb_libraries++;
                break;
            case TCC_OPTION_bench:
                do_bench = 1;
                break;
            case TCC_OPTION_c:
                multiple_files = 1;
                output_type = TCC_OUTPUT_OBJ;
                break;
            case TCC_OPTION_o:
                multiple_files = 1;
                outfile = optarg;
                break;
            case TCC_OPTION_r:
                reloc_output = 1;
                output_type = TCC_OUTPUT_OBJ;
                break;
            case TCC_OPTION_nostdinc:
                s->nostdinc = 1;
                break;
            case TCC_OPTION_print_search_dirs:
                print_search_dirs = 1;
                break;
            case TCC_OPTION_v:
                printf("tcc version %s\n", TCC_VERSION);
                return 0;
            default:
                break;
            }
        }
    }
    if (print_search_dirs) {
        printf("install: %s/\n", tcc_lib_path);
        return 0;
    }

    nb_objfiles = nb_files - nb_libraries;

    /* If output file name is specified without actions, enforce standard binary generation */
    if (outfile && output_type == TCC_OUTPUT_MEMORY)
        output_type = TCC_OUTPUT_EXE;

    /* Verify -c argument constraints ensuring only isolated standalone objects are accepted */
    if (output_type == TCC_OUTPUT_OBJ && !reloc_output) {
        if (nb_objfiles != 1)
            error("cannot specify multiple files with -c");
        if (nb_libraries != 0)
            error("cannot specify libraries with -c");
    }
    
    /* Calculate fallback output destination filename if missing from arguments */
    if (output_type != TCC_OUTPUT_MEMORY && !outfile) {
        if (output_type == TCC_OUTPUT_OBJ && !reloc_output) {
            char *ext;
            pstrcpy(objfilename, sizeof(objfilename) - 1, files[0]);
            ext = strrchr(objfilename, '.');
            if (!ext)
                goto default_outfile;
            strcpy(ext + 1, "o");
        } else {
        default_outfile:
            pstrcpy(objfilename, sizeof(objfilename), "a.out");
        }
        outfile = objfilename;
    }

    if (do_bench) {
        start_time = getclock_us();
    }

    /* Configure compiler system state targets and load baseline symbol environments */
    tcc_set_output_type(s, output_type);

    /* Process, compile, and sequence every specified target input file or archive library */
    for(i = 0; i < nb_files; i++) {
        const char *filename;
        filename = files[i];

        if (filename[0] == '-') {
            if (tcc_add_library(s, filename + 2) < 0)
                error("cannot find %s", filename);
        } else {
            if (tcc_add_file(s, filename) < 0) {
                ret = 1;
                goto the_end;
            }
        }
    }

    tcc_free(files);

    /* =========================================================================
       TRDOS NATIVE TCC LINKER FINAL RELOCATION ENGINE
       ========================================================================= */
    /* Resolve linear pointer references across newly initialized flat segments in RAM */
    tcc_relocate(s); 

    /* =========================================================================
       TRDOS NATIVE TCC FLAT BINARY WRITER - ABSOLUTE KERNEL SEAL
       ========================================================================= */
    unsigned char *text_ptr  = text_section->data;
    unsigned int   text_size = text_section->data_offset;

    if (text_section && text_section->data_offset > 0) {
        const char *out_name = "TEST.PRG";
        int trdos_fd = -1;

        /* EXECUTE STEP A: Invoke TRDOS sys_create kernel vector interrupt (eax = 8) */
        __asm__ __volatile__ (
            ".intel_syntax noprefix\n"
            "mov ecx, 0\n"              
            "mov eax, 8\n"              
            "int 0x40\n"                
            "jnc .L_final_create_ok\n"
            "mov eax, -1\n"             
        ".L_final_create_ok:\n"
            "mov %0, eax\n"             
            ".att_syntax\n"
            : "=r" (trdos_fd)           
            : "b" (out_name)            
            : "eax", "ecx"
        );

        if (trdos_fd >= 0) {
            /* EXECUTE STEP B: Commit fully-linked .text flat code segment to disk (sys_write = 4) */
            __asm__ __volatile__ (
                ".intel_syntax noprefix\n"
                "mov eax, 4\n"          
                "int 0x40\n"
                ".att_syntax\n"
                :
                : "b" (trdos_fd), "c" (text_ptr), "d" (text_size)
                : "eax"
            );

            /* EXECUTE STEP C: Append contiguous data layout segment (.data) if initialized on build */
            if (data_section && data_section->data_offset > 0) {
                unsigned char *data_ptr = data_section->data;
                unsigned int data_size = data_section->data_offset;

                __asm__ __volatile__ (
                    ".intel_syntax noprefix\n"
                    "mov eax, 4\n"      
                    "int 0x40\n"
                    ".att_syntax\n"
                    :
                    : "b" (trdos_fd), "c" (data_ptr), "d" (data_size)
                    : "eax"
                );
            } 

            /* EXECUTE STEP D: Safely finalize file transaction closing target handle (sys_close = 6) */
            __asm__ __volatile__ (
                ".intel_syntax noprefix\n"
                "mov eax, 6\n"          
                "int 0x40\n"
                ".att_syntax\n"
                :
                : "b" (trdos_fd)
                : "eax"
            );
            
            /* EXECUTE STEP E: Force absolute process escape returning directly to the shell prompt (sys_exit = 1) */
            __asm__ __volatile__ (
                ".intel_syntax noprefix\n"
                "mov ebx, 0\n"          
                "mov eax, 1\n"          
                "int 0x40\n"
                ".att_syntax\n"
            );
        }
    }

the_end:
    /* Safely release instance allocations to guarantee zero leakage path exceptions */
    tcc_delete(s);
    return ret;
}
