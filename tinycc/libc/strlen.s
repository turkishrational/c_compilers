.intel_syntax noprefix
.global _strlen
.text

_strlen:
    push ebp
    mov ebp, esp
    push edi

    mov edi, [ebp + 8]
    sub eax, eax

.L_strlen_loop:
    cmp byte ptr [edi], 0
    je .L_strlen_done
    inc edi
    inc eax
    jmp .L_strlen_loop

.L_strlen_done:
    pop edi
    pop ebp
    ret
