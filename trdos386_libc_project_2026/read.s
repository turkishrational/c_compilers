; ****************************************************************************
; read.s - TRDOS 386 C Standard Library (LIBC 2026)
; ----------------------------------------------------------------------------
; Input Stream & File Reading Logic for C Compilers (GCC 1.27 / TCC) Porting
; Implementation of: int read(int fd, void *buf, int count)
; ****************************************************************************

global _read

[BITS 32]

_read:
    push    ebx             ; Preserve EBX according to C calling convention (cdecl)

    mov     ebx, [esp+8]    ; Argument 1: LIBC File Descriptor (fd)
    mov     ecx, [esp+12]   ; Argument 2: Destination Buffer Pointer (buf)

    ; 1. Standard Input/Output Stream Check
    cmp     ebx, 3
    jb      .read_stdio     ; If fd < 3 (stdin/out/err), route to sys_stdio (46)

    ; 2. Regular File Reading Routine (fd >= 3)
    sub     ebx, 3          ; Convert LIBC FD (3-12) to Kernel FD (0-9)
    mov     edx, [esp+16]   ; Argument 3: Max Byte Count to Read (count)
    mov     eax, 3          ; sys_read (EAX = 3)
    int     40h             ; Call TRDOS 386 Kernel
    jnc     .done           ; If CF=0, EAX contains read bytes or 0, return it

.fail:
    mov     eax, -1         ; On kernel error, return standard C error (-1)
    pop     ebx             ; Restore EBX
    retn

.read_stdio:
    ; For reading, only stdin (fd=0, BL=0) is valid. fd=1 or fd=2 are invalid.
    cmp     bl, 1           ; Check if fd is 1 or 2
    cmc                     ; Complement Carry Flag
    jc      .fail           ; If fd >= 1, abort with error (-1)
    
    xor     edx, edx        ; Clear character counter (edx = 0)

.read_stdio_next:
    mov     eax, 46         ; sys_stdio (EAX = 46, Sub-function: BL=0 for STDIN)
    int     40h             ; Call TRDOS 386 Kernel
    jc      .ok             ; If error or EOF, stop reading loop

    mov     [ecx], al       ; Store the retrieved character into buffer
    and     al, al          ; Is it a null terminator?
    jz      .ok             ; If null, finish reading
    
    inc     edx             ; Increment successfully read character count
    cmp     edx, [esp+16]   ; Check against requested count limit (Offset is +16)
    jnb     .ok             ; If limit reached or exceeded, stop
    
    inc     ecx             ; Advance destination buffer pointer
    cmp     al, 27          ; Check for ESC key (ASCII 27)
    je      .ok             ; If ESC pressed, break loop
    cmp     al, 13          ; Check for Enter/CR key (ASCII 13)
    je      .ok             ; If Enter pressed, break loop
    jmp     .read_stdio_next

.ok:
    mov     eax, edx        ; Return the total number of characters read

.done:
    pop     ebx             ; Restore EBX
    retn
