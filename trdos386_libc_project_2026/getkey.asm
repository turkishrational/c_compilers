;nasm

; short get_key_ext(void) 
; AH = Scancode, AL = Ascii döner
global _get_key_ext
_get_key_ext:
    mov eax, 7
    mov bl, 6           ; BL=6 -> Read char (ascii+scan) wait
    int 40h
    ; EAX içinde AL:ASCII, AH:SCANCODE hazýr döner
    ret
