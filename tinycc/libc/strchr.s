.intel_syntax noprefix
.global _strchr
.text

_strchr:
    push ebp
    mov ebp, esp
    push edi

    mov edi, [ebp + 8]
    mov eax, [ebp + 12]

.L_strchr_loop:
    mov cl, [edi]
    cmp cl, al
    je .L_strchr_found
    cmp cl, 0
    je .L_strchr_notfound
    inc edi
    jmp .L_strchr_loop

.L_strchr_found:
    mov eax, edi
    jmp .L_strchr_done

.L_strchr_notfound:
    xor eax, eax

.L_strchr_done:
    pop edi
    pop ebp
    ret
