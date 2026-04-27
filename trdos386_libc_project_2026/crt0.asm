; -------------------------------------------------------------------------
; TRDOS 386 - CRT0.ASM (Windows 7 / NASM)
; -------------------------------------------------------------------------

[BITS 32]

section .text
global __start
extern _main

__start:
    ; TRDOS 386 Stack: [ESP] = argc, [ESP+4] = argv[0]
    ; Stack adresi sysexec tarafýndan 4GB-4MB-8byte olarak ayarlanýr.
    
    mov eax, [esp]      ; argc
    lea ebx, [esp+4]    ; argv pointer
    
    push ebx            ; argv
    push eax            ; argc
    
    call _main
    
    ; Çýkýþ: EAX'te main'den dönen deðer var
    mov ebx, eax        ; exit code
    mov eax, 1          ; sys_exit
    int 0x40
    
    hlt                 ; Durdur