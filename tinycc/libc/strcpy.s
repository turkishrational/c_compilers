.intel_syntax noprefix
.global _strcpy
.text

_strcpy:
    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, [ebp + 8]
    mov esi, [ebp + 12]

.L_strcpy_loop:
    mov al, [esi]
    mov [edi], al
    cmp al, 0
    je .L_strcpy_done
    inc esi
    inc edi
    jmp .L_strcpy_loop

.L_strcpy_done:
    mov eax, [ebp + 8]
    pop esi
    pop edi
    pop ebp
    ret
