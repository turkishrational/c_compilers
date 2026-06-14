; ****************************************************************************
; close.s - TRDOS 386 C Standard Library (LIBC 2026)
; ----------------------------------------------------------------------------
; File Descriptor Closing Logic for C Compilers (GCC 1.27 / TCC) Porting Project
; Implementation of: int close(int fd)
; ****************************************************************************

global _close

[BITS 32]

_close:
    push    ebx             ; Preserve EBX according to C calling convention (cdecl)

    mov     eax, [esp+8]    ; Get argument 1: LIBC File Descriptor (fd) 
                            ; (Offset is +8 now because of the push ebx)
    sub     eax, 3          ; Shift LIBC FD (3-12) back to Kernel FD range (0-9)
    jb      .ignore         ; If fd < 3 (stdin/out/err), ignore the close request

    mov     ebx, eax        ; EBX = Kernel File Descriptor (0-9)
    mov     eax, 6          ; sys_close (EAX = 6)
    int     40h             ; Call TRDOS 386 Kernel
    jc      .err            ; If carry flag is set, kernel returned an error

    ;xor    eax, eax        ; Return 0 on success
    ;pop    ebx             ; Restore EBX
    ;retn

.ignore:
    xor     eax, eax        ; Fake success for standard I/O streams, return 0
    pop     ebx             ; Restore EBX
    retn

.err:
    mov     eax, -1         ; Return standard C error indicator (-1)
    pop     ebx             ; Restore EBX
    retn