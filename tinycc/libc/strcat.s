.intel_syntax noprefix
.global _strcat
.text

_strcat:
    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, [ebp + 8]    /* Parameter 1: dest string address */
    mov esi, [ebp + 12]   /* Parameter 2: src string address */

.L_find_end:
    cmp byte ptr [edi], 0
    je .L_copy_start
    inc edi
    jmp .L_find_end

.L_copy_start:
    mov dl, [esi]
    mov [edi], dl
    cmp dl, 0
    je .L_strcat_done
    inc esi
    inc edi
    jmp .L_copy_start

.L_strcat_done:
    mov eax, [ebp + 8]    /* Return original dest address per C standard */
    pop esi
    pop edi
    pop ebp
    ret

