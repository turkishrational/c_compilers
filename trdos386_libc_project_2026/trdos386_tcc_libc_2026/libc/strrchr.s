.intel_syntax noprefix
.global _strrchr
.text

_strrchr:
    push ebp
    mov ebp, esp
    push edi

    mov edi, [ebp + 8]
    mov ecx, [ebp + 12]
    xor eax, eax

.L_strrchr_loop:
    mov dl, [edi]
    cmp dl, cl
    jne .L_strrchr_check_null
    mov eax, edi

.L_strrchr_check_null:
    cmp dl, 0
    je .L_strrchr_done
    inc edi
    jmp .L_strrchr_loop

.L_strrchr_done:
    pop edi
    pop ebp
    ret

