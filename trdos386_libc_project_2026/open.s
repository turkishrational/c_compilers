; ****************************************************************************
; open.s - TRDOS 386 C Standard Library (LIBC 2026)
; ----------------------------------------------------------------------------
; Smart Open/Creat Logic for C Compilers (GCC 1.27 / TCC) Porting Project
; Implementation of: int open(const char *path, int flags, int mode)
; ****************************************************************************

global _open

[BITS 32]

_open:
    push    ebx
    push    esi
    
    mov     ebx, [esp+12]   ; Argument 1: path string pointer
    mov     esi, [esp+16]   ; Argument 2: C flags (0=R, 1=W, 2=RW, +0x40=CREAT, +0x200=TRUNC)
    
    ; 1. Intent Check: Read-Only Mode? (O_RDONLY = 0)
    test    esi, esi        ; Are flags set to 0?
    jz      .do_sys_open    ; If read-only, execute standard sys_open directly

    ; 2. Intent Check: Write operation with Truncate (Overwrite)?
    ; In C standard, "w" mode sends O_WRONLY | O_CREAT | O_TRUNC (0x241)
    test    esi, 0x200      ; Is O_TRUNC (512) flag set?
    jnz     .do_sys_creat   ; If set, jump to sys_creat to truncate/create file

    ; 3. Intent Check: Write operation without Truncate (O_APPEND or modifying existing data)
    ; Check for O_CREAT flag to prevent errors if the file does not exist.
    test    esi, 0x40       ; Is O_CREAT (64) flag set?
    jz      .do_sys_open_write ; If NO O_CREAT, try opening existing file in write mode

    ; 4. Mixed Intent: Create file if missing, but do not destroy if exists (O_CREAT | O_EXCL)
    ; Try sys_open first; if file is missing, fallback to sys_creat.
    mov     eax, 5          ; sys_open (EAX = 5)
    mov     ecx, 1          ; Write mode (ECX = 1)
    int     40h             ; Call TRDOS 386 Kernel
    jnc     .success_fd     ; If file exists and opened successfully, skip to adjustment

    cmp     eax, 2          ; Error evaluation: File not found? (TRDOS Error Code: 2)
    je      .do_sys_creat   ; If not found, jump to creation routine
    jmp     .err            ; For any other error (e.g. Access Denied), abort and return -1

.do_sys_creat:
    mov     eax, 8          ; sys_creat (EAX = 8, Truncate/Create file)
    mov     ecx, 0          ; File attributes: Normal file (ECX = 0)
    int     40h             ; Call TRDOS 386 Kernel
    jmp     .handle_res

.do_sys_open_write:
    mov     eax, 5          ; sys_open (EAX = 5)
    mov     ecx, 1          ; Write mode (ECX = 1)
    int     40h             ; Call TRDOS 386 Kernel
    jmp     .handle_res

.do_sys_open:
    mov     eax, 5          ; sys_open (EAX = 5)
    mov     ecx, 0          ; Read mode (ECX = 0)
    int     40h             ; Call TRDOS 386 Kernel

.handle_res:
    jc      .err            ; If carry flag is set, kernel returned an error
.success_fd:
    add     eax, 3          ; Shift kernel FD (0-9) to LIBC FD range (3-12)
    jmp     .done

.err:
    mov     eax, -1         ; Return standard C error indicator (-1)
.done:
    pop     esi
    pop     ebx
    retn
