/* ===========================================================================
   TRDOS 386 Port - Fixed mingw_hack.s (Real realloc simulation)
   =========================================================================== */
.intel_syntax noprefix
.global ___mingw_free
.global ___mingw_realloc
.extern _sbrk
.extern _memcpy
.text

___mingw_free:
    ret

___mingw_realloc:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi

    mov ebx, [ebp + 8]    /* Argument 1: old_ptr */
    mov ecx, [ebp + 12]   /* Argument 2: new_size */

    /* If old_ptr is NULL, realloc behaves exactly like malloc */
    test ebx, ebx
    jz .L_alloc_new

    /* Request the new full size directly via _malloc */
    push ecx
    call _malloc
    add esp, 4
    mov edi, eax          /* EDI = New allocated address space pointer */
    
    test edi, edi
    jz .L_realloc_fail

    /* Copy old data to the new space */
    push ecx              /* Count: new_size (contains 100% of the old data) */
    push ebx              /* Source: old_ptr */
    push edi              /* Destination: new_ptr */
    call _memcpy
    add esp, 12

    mov eax, edi          /* Return the successfully expanded new pointer address */
    jmp .L_realloc_done

.L_alloc_new:
    push ecx
    call _malloc
    add esp, 4
    jmp .L_realloc_done

.L_realloc_fail:
    xor eax, eax          /* Return NULL (0) on serious allocation failure */

.L_realloc_done:
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
