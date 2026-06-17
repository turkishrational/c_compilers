.intel_syntax noprefix
.global _sbrk
.extern _end
.text

_sbrk:
    push ebp
    mov ebp, esp

    /* Load the tracked dynamic memory boundary */
    mov eax, [current_brk] 
    test eax, eax
    jnz .L_do_inc             /* If already initialized, jump to increment */

    /* First-time initialization: Set heap start where Linker marked the end (_end) */
    mov eax, offset _end      /* EAX = Program Size + BSS End Pointer */
    mov [current_brk], eax    /* Initialize tracker storage */

.L_do_inc:
    mov edx, [ebp + 8]        /* Fetch argument 1: increment size (bytes) */
    
    /* EAX preserves the OLD break address to be returned back to malloc */
    add edx, eax              /* Calculate raw target address boundary */
    add edx, 3                /* Align to 32-bit (DWORD) boundary: Round up */
    ;and edx, 0xFFFFFFFC      /* Clear lower two bits to finalize 4-byte alignment */
    and dl, 0xFC 	    

    mov [current_brk], edx    /* Save the newly aligned dynamic memory limit */
    
    /* EAX currently contains the unmodified previous break address, which is the return value */
    pop ebp
    ret

/* ---------------------------------------------------------------------------- */
/* Internal Dynamic Storage (Embedded inside text segment via GAS directives)   */
/* ---------------------------------------------------------------------------- */
.align 4
current_brk: 
    .long 0                   /* Runtime variable holding current heap limit */
