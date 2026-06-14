; -----------------------------------------------------------
; TRDOS 386 - Crt0.o template - Erdogan tan - 14/04/2026
; -----------------------------------------------------------
; nasm -Fo CRT0.ASM
;
; https://github.com/adamsch1/scc (smc386c.c)

extern _main

;B+ System header
	use32
  	jmp	START	; TRDOS 386 PRG file doesn't contain header

  	db  "CRT0"
START:
  	; esp = argc
  	; esp+4 = argv[0] address
	pop	eax	; argc (value)
	mov	ebx,esp	; argv[0] address (not value)
	push	eax	; argc
	push	ebx

	call	_main
	;add	esp,8
	pop	ebx
  	;
	xor	ebx,ebx	; exit code = 0
	mov	eax,1	; sysexit
	int	40h

	db 0	
	db "C Compiler v1.0 for TRDOS 386"
	db 0
	db "Erdogan Tan - 2026"
	db 0