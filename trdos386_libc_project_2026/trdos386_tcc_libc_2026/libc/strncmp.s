.intel_syntax noprefix
.global _strncmp
.text

_strncmp:
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]
    mov ecx, [ebp + 16]

    xor eax, eax
    test ecx, ecx
    jz .L_strncmp_done

.L_strncmp_loop:
    mov al, [esi]
    mov dl, [edi]
    cmp al, dl
    jne .L_strncmp_diff
    cmp al, 0
    je .L_strncmp_done
    inc esi
    inc edi
    dec ecx
    jnz .L_strncmp_loop
    jmp .L_strncmp_done

.L_strncmp_diff:
    movzx eax, al
    movzx edx, dl
    sub eax, edx

.L_strncmp_done:
    pop edi
    pop esi
    pop ebp
    ret

