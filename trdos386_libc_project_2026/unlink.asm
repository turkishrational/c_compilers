;nasm

; int unlink(const char *pathname)
global _unlink
_unlink:
    push ebx
    mov ebx, [esp+8]    ; pathname
    mov eax, 10         ; sys_delete (trdosk6.s)
    int 40h
    jc .err
    xor eax, eax        ; Baþarýlý: 0
    jmp .exit
.err:
    mov eax, -1
.exit:
    pop ebx
    ret
