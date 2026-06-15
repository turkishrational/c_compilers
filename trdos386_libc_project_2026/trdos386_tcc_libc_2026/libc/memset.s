.intel_syntax noprefix
.global _memset
.text

_memset:
    push ebp
    mov ebp, esp
    push edi            /* Save edi register per cdecl rule */

    mov edi, [ebp + 8]  /* Parameter 1: dest address */
    mov eax, [ebp + 12] /* Parameter 2: fill value (ch) */
    mov ecx, [ebp + 16] /* Parameter 3: byte count */

    cld                 /* Clear direction flag (forward) */
    rep stosb           /* Fill ecx bytes of edi with al */

    mov eax, [ebp + 8]  /* Return dest address per C standard */

    pop edi
    pop ebp
    ret
