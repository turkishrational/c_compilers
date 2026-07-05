; ****************************************************************************
; tccexec.s - TRDOS 386 - TCC FLAT PRG EXECUTION & EXIT CODE MONITOR
; ----------------------------------------------------------------------------
; Erdoğan Tan & Google AI - 04/07/2026
; ****************************************************************************

; TRDOS 386 Sistem Çağrı Sabitleri
_exit 	equ 1
_fork 	equ 2
_wait 	equ 7
_exec	equ 11
_msg    equ 35
_stdio  equ 46

%macro sys 1-4
    %if %0 >= 2   
        mov ebx, %2
        %if %0 >= 3    
            mov ecx, %3
            %if %0 = 4
               mov edx, %4   
            %endif
        %endif
    %endif
    mov eax, %1
    int 40h 
%endmacro

[BITS 32] 
[ORG 0] 

START_CODE:
	; Başlangıç başlığını bas (Kırmızı Renk - 0Ch)
	mov	byte [color], 0Ch
	mov 	esi, tcc_monitor_hdr
	call 	print_msg

	; Süreci çatalla (Fork)
	mov 	ebx, child_launcher
	sys 	_fork
	jc 	short execution_error

	mov	[cpid], eax 	; Alt sürecin ID'sini sakla

	; Parent bekleme döngüsüne giriyor
	mov	byte [color], 07h
parent_wait:
	mov	ebx, 999 	; Bloklanan wait çağrısı
	sys 	_wait
	jnc	short wait_success

execution_error:
	mov 	esi, msg_err
	call 	print_msg
	sys	_exit

wait_success:
	cmp 	eax, [cpid]	; Doğru child mı kontrol et
	jne 	short parent_wait	

	; EAX = Alt sürecin ID'si
	; EBX = Alt sürecin dönüş kodu (Exit Code) -> TCC'nin ürettiği 'c' değeri!
	push	ebx  		; Çıkış kodunu yığına at

	; Başarı mesajlarını ve dosya adını yazdır (Yeşil/Sarı)
	mov	byte [color], 0Ah
	mov	esi, child_label
	call	print_msg
	mov	byte [color], 0Eh
	mov	esi, prgfilename
	call	print_msg	

	mov	byte [color], 07h
	mov	esi, CRLF
	call	print_msg

	; Yığından exit code değerini al ve ekrana basmak için ASCII'ye çevir
	pop	eax
	call 	bin_to_decimal_str
	mov 	[exitcode], ebx

	mov	byte [color], 0Fh 	; Parlak Beyaz
	mov 	esi, child_exitcode_msg
	call 	print_msg

monitor_ok:
	mov	byte [color], 0Ah
	mov 	esi, msg_ok
	call 	print_msg

	sys	_exit

; ============================================================================
; CHILD LAUNCHER - ALT SÜREÇ TETİKLEYİCİSİ
; ============================================================================
child_launcher:
	; TCC tarafından üretilen gerçek 'TEST.PRG' dosyasını EXEC et!
	sys	_exec, prgfilename, prgp
	jnc	short child_exit_direct

	; Eğer dosya bulunamadıysa hata bas
	cmp	al, 2 
	jne	execution_error

	mov	byte [color], 0Ch
	mov	esi, prgfilename
	call	print_msg
	mov	esi, not_found
	call	print_msg

child_fail_exit:
	mov	eax, 0xFF		; Hata durumunda 255 döndür
child_exit_direct:
	mov	ebx, eax
	sys 	_exit

; ============================================================================
; YARDIMCI FONKSİYONLAR
; ============================================================================
print_msg:
	sys 	_msg, esi, 255, [color]
	retn

bin_to_decimal_str:
	mov	ecx, 10
	xor	ebx, ebx
btd_loop:
	xor	edx, edx
  	div	ecx
	shl	ebx, 8
	add	dl, '0'
	mov	bl, dl
	or	eax, eax
	jnz	short btd_loop
	retn

; ============================================================================
; VERİ VE TANIMLAMALAR BÖLGESİ
; ============================================================================
color:		dd 0
cpid:		dd 0

; Çalıştırılacak hedef TCC flat binary dosyası
prgfilename:	db "TEST.PRG", 0

prgp:		dd prgfilename
		dd 0
		dd 0

; İletiler
tcc_monitor_hdr:
	db 0Dh, 0Ah
	db 'TRDOS 386 v2 - TCC Flat PRG Exit Code Monitor'
	db 0Dh, 0Ah, 0

child_label:
	db 0Dh, 0Ah
	db "Executed Program: ", 0

not_found:
	db " not found on disk! ", 0Dh, 0Ah, 0

child_exitcode_msg:
	db '>>> TCC MAIN() RETURN VALUE (EXIT CODE): '
exitcode:
	dd 30303030h  ; ASCII yerleşim alanı
	db 0Dh, 0Ah, 0

msg_err:
	db 0Dh, 0Ah 
	db 'Execution Error!'
	db 0Dh, 0Ah, 0

msg_ok:
	db 0Dh, 0Ah
	db 'Monitor Finished Successfully. ', 0Dh, 0Ah, 0
CRLF:
	db 0Dh, 0Ah, 0
