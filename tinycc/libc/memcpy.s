.intel_syntax noprefix
.global _memcpy
.text

_memcpy:
    push ebp
    mov ebp, esp
    push edi            /* Save edi register */
    push esi            /* Save esi register */

    mov edi, [ebp + 8]  /* Parameter 1: dest address */
    mov esi, [ebp + 12] /* Parameter 2: src address */
    mov ecx, [ebp + 16] /* Parameter 3: byte count */

    cld                 /* Clear direction flag (forward) */
    rep movsb           /* Copy ecx bytes from esi to edi */

    mov eax, [ebp + 8]  /* Return dest address per C standard */

    pop esi
    pop edi
    pop ebp
    ret
