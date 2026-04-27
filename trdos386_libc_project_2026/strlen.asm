;nasm

global _strlen
_strlen:
    push edi
    mov edi, [esp+8]    ; string pointer
    xor al, al          ; null char
    mov ecx, 0xFFFFFFFF
    repne scasb         ; null karakteri ara
    mov eax, 0xFFFFFFFE
    sub eax, ecx        ; uzunluðu hesapla
    pop edi
    ret
