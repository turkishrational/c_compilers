.intel_syntax noprefix
.global _memmove
.text

_memmove:
    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, [ebp + 8]
    mov esi, [ebp + 12]
    mov ecx, [ebp + 16]

    cmp edi, esi
    jbe .L_move_forward
    
    lea edi, [edi + ecx - 1]
    lea esi, [esi + ecx - 1]
    std
    rep movsb
    cld
    jmp .L_move_done

.L_move_forward:
    cld
    rep movsb

.L_move_done:
    mov eax, [ebp + 8]
    pop esi
    pop edi
    pop ebp
    ret
