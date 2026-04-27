; nasm
; -----------------------------------------------------------
; TRDOS 386 - CRT0 (C Runtime Startup) - Erdogan Tan
; -----------------------------------------------------------

[BITS 32]

SECTION .text
global _start
extern _main
extern _exit

_start:
    ; Program yüklendiðinde:
    ; [esp]   = argc
    ; [esp+4] = argv[0] pointer
    ; [esp+8] = argv[1] pointer ...
    
    mov eax, [esp]     ; argc
    lea ebx, [esp+4]   ; argv (pointer to array)
    
    push ebx           ; argv push
    push eax           ; argc push
    
    call _main
    
    ; Main biterse sonucu exit'e gönder
    push eax
    call _exit

; Çýkýþ güvenliði için yedek sysexit
loop_dead:
    xor ebx, ebx
    mov eax, 1         ; sysexit
    int 40h
    jmp loop_dead