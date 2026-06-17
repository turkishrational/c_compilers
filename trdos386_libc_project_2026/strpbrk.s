.intel_syntax noprefix
.global _strpbrk
.text

_strpbrk:
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov esi, [ebp + 8]    /* Parameter 1: str1 pointer */
    
.L_s1_loop:
    mov al, [esi]
    cmp al, 0
    je .L_pbrk_null       /* End of str1 reached without any match */
    
    mov edi, [ebp + 12]   /* Parameter 2: str2 (accept characters) */
.L_s2_loop:
    mov dl, [edi]
    cmp dl, 0
    je .L_s1_next
    cmp al, dl
    je .L_pbrk_found      /* Found a matching character */
    inc edi
    jmp .L_s2_loop

.L_s1_next:
    inc esi
    jmp .L_s1_loop

.L_pbrk_found:
    mov eax, esi          /* Return pointer to the matched character in str1 */
    jmp .L_pbrk_done

.L_pbrk_null:
    xor eax, eax          /* Return NULL pointer */

.L_pbrk_done:
    pop edi
    pop esi
    pop ebp
    ret
