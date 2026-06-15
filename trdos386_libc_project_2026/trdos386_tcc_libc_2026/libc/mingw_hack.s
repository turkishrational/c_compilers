.intel_syntax noprefix
.global ___mingw_free
.global ___mingw_realloc
.text

/* 
   Explicitly defining triple underscores in GAS so that it exports 
   exactly double underscores "__mingw_free" to the Win32 COFF linker.
*/

___mingw_free:
    /* tcc calls this to free memory blocks. For now, we stub it safely. */
    ret

___mingw_realloc:
    /* Returns the original pointer passed in as argument 1 (esp + 4) */
    mov eax, [esp + 4]
    ret
