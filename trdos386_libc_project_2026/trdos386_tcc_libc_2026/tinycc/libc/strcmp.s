.intel_syntax noprefix
.global _strcmp
.text

_strcmp:
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]

.L_strcmp_loop:
    mov al, [esi]
    mov dl, [edi]
    cmp al, dl
    jne .L_strcmp_diff
    cmp al, 0
    je .L_strcmp_equal
    inc esi
    inc edi
    jmp .L_strcmp_loop

.L_strcmp_diff:
    movzx eax, al
    movzx edx, dl
    sub eax, edx
    jmp .L_strcmp_done

.L_strcmp_equal:
    xor eax, eax

.L_strcmp_done:
    pop edi
    pop esi
    pop ebp
    ret

