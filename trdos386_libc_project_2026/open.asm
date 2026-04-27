;nasm

; int open(const char *path, int flags, ...)
global _open
_open:
    push ebx
    mov ebx, [esp+8]    ; path
    mov edx, [esp+12]   ; flags (C-style: 0=RD, 1=WR, 0x40=CREAT)

    mov ecx, 0          ; Mod: Okuma

    ; Basit bir kontrol: Yazma veya Oluþturma isteniyor mu?
    test edx, 0x40      ; O_CREAT (GCC/TCC standardý)
    jnz .do_create

    test edx, 1
    jz short .open_r
    inc ecx		; open for write

.open_r:
    mov eax, 5          ; sys_open
    int 40h
    jmp .check_ret

.do_create:
    ;mov ecx, 0		; normal file (atributes = 0)
    mov eax, 8          ; sys_create (dosya yoksa oluþturur)
    int 40h

.check_ret:
    jc .err
    add eax, 3          ; Kernel FD -> Libc FD (3, 4, 5...)
    pop ebx
    ret

.err:
    cmp	eax, 2		; "file not found"
    je .do_create
    mov eax, -1
    pop ebx
    ret


