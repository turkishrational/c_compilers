.intel_syntax noprefix
.global _memcmp
.text

_memcmp:
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]
    mov ecx, [ebp + 16]

    sub eax, eax
    cld
    repe cmpsb
    je .L_memcmp_done

    mov al, [esi - 1]
    mov dl, [edi - 1]
    movzx eax, al
    movzx edx, dl
    sub eax, edx

.L_memcmp_done:
    pop edi
    pop esi
    pop ebp
    ret

