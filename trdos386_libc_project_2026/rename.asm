;nasm

; int rename(const char *oldpath, const char *newpath)
global _rename
_rename:
    push ebx
    push esi            ; oldpath için esi, newpath için edi gerekebilir
    mov ebx, [esp+12]   ; oldpath
    mov ecx, [esp+16]   ; newpath
    mov eax, 9          ; sys_rename (trdosk6.s)
    int 40h
    jc .err
    xor eax, eax
    jmp .exit
.err:
    mov eax, -1
.exit:
    pop esi
    pop ebx
    ret
