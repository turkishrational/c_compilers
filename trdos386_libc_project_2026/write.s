; ****************************************************************************
; write.s - TRDOS 386 C Standard Library (LIBC 2026)
; ----------------------------------------------------------------------------
; Output Stream & File Writing Logic for C Compilers (GCC 1.27 / TCC) Porting
; Implementation of: int write(int fd, const void *buf, int count)
; ****************************************************************************

global _write

[BITS 32]

_write:
    push    ebx             ; Preserve EBX (POSIX / C Calling Convention requirement)
    push    esi             ; Preserve ESI (Callee-saved register)
                            ; Stack dropped by 8 bytes total!

    ; Original offsets before pushes: fd=[esp+4], buf=[esp+8], count=[esp+12]
    ; New offsets after 8-byte stack shift:
    mov     ebx, [esp+12]   ; Argument 1: LIBC File Descriptor (fd)
    cmp     ebx, 3
    jb      .std_out        ; If fd < 3 (stdin/out/err), route to sys_stdio (46)

    ; 1. Regular File Writing Routine (fd >= 3)
    sub     ebx, 3          ; Convert LIBC FD (3-12) to Kernel FD (0-9)
    mov     ecx, [esp+16]   ; Argument 2: Source Buffer Pointer (buf)
    mov     edx, [esp+20]   ; Argument 3: Byte Count to Write (count)
    mov     eax, 4          ; sys_write (EAX = 4)
    int     40h             ; Call TRDOS 386 Kernel
    jnc     .success        ; If CF=0, EAX contains written bytes, branch to exit

.fail:
    mov     eax, -1         ; On error, return standard C error indicator (-1)
    jmp     .done

.std_out:
    ; Validate fd for write operations: 1 (stdout) or 2 (stderr) are valid.
    ; fd=0 (stdin) is invalid for writing.
    cmp     bl, 1           ; Check if fd is at least 1 (stdout)
    jb      short .fail     ; If fd == 0 (stdin), abort with error (-1)
    
    xor     edx, edx        ; Clear character counter (edx = 0)
    inc     ebx             ; Map LIBC FD (1 or 2) to sys_stdio sub-functions:
                            ; BL=2 (stdout) or BL=3 (stderr)

    mov     esi, [esp+16]   ; Fetch buf pointer (Offset shifted to +16)

.std_out_next:
    lodsb                   ; Load next character from [ESI] into AL, and inc ESI
    mov     cl, al          ; Move character into CL as required by sys_stdio
    mov     eax, 46         ; sys_stdio (EAX = 46)
    int     40h             ; Call TRDOS 386 Kernel
    jc      short .ok       ; If carry flag is set (kernel error), break loop
    
    inc     edx             ; Increment successfully written character count
    cmp     edx, [esp+20]   ; Check against count limit (Offset shifted to +20)
    jb      .std_out_next   ; Loop until all characters are processed

.ok:
    mov     eax, edx        ; Return the total number of characters written
    jmp     .done

.success:
    ; EAX already contains the number of bytes written by the kernel
.done:
    pop     esi             ; Restore ESI
    pop     ebx             ; Restore EBX
    retn