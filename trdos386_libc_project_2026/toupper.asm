;nasm

global _toupper
_toupper:
    mov eax, [esp+4]
    cmp eax, 'a'
    jb .done
    cmp eax, 'z'
    ja .done
    sub eax, 32         ; Küçük harfi büyük yap
.done:
    ret