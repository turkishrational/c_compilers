.intel_syntax noprefix
.global _system
.global system
.global ___p__environ
.global _GetProcAddress@8
.global _LoadLibraryA@4
.global _FreeLibrary@4
.global _VirtualProtect@16
.global _SetUnhandledExceptionFilter@4
.global _SearchPathA@24

/* TCC El Sýkýþma / TRDOS LIBC Standart Köprü Giriþleri */
.global _fopen
.global fopen
.global _fclose
.global fclose

/* Initial Versiyon Eksik Sembol Giriþleri */
.global _ungetc
.global ungetc
.global _puts
.global puts

.text

_system:
system:
    mov eax, -1       /* TRDOS altýnda süreç tetikleme kapalý */
    ret

___p__environ:
    xor eax, eax
    ret

/* =========================================================================
   1. SAF ASSEMBLER _fopen ÝNÞASI (+3 LIBC FD ZIRHI)
   ========================================================================= */
_fopen:
fopen:
    push ebp
    mov ebp, esp
    push ebx
    push ecx

    mov eax, [ebp + 12] /* Mode dizesinin adresi ("r", "w") */
    mov ebx, [ebp + 8]  /* Filename dizesinin adresi */

    cmp byte ptr [eax], 114 /* 'r' karakteri mi? (ASCII 114) */
    je .L_open_read
    cmp byte ptr [eax], 119 /* 'w' karakteri mi? (ASCII 119) */
    je .L_open_write

.L_bad_mode:
    xor eax, eax        /* Hata veya geçersiz mod: Return NULL (0) */
    jmp .L_fopen_done

.L_open_read:
    /* Saf TRDOS sys_open (EAX=5) çaðrýsý */
    /* EBX = filename, ECX = 0 (open for read) */
    xor ecx, ecx
    mov eax, 5
    int 0x40
    jc .L_bad_mode

    add eax, 3          /* Kernel FD (0-9) -> LIBC FD (3-12) zýrhý */
    jmp .L_fopen_done

.L_open_write:
    /* Saf TRDOS sys_creat (EAX=8) çaðrýsý */
    /* EBX = filename, ECX = 0 (ordinary file) */
    xor ecx, ecx
    mov eax, 8
    int 0x40
    jc .L_bad_mode

    add eax, 3          /* Kernel FD (0-9) -> LIBC FD (3-12) zýrhý */

.L_fopen_done:
    pop ecx
    pop ebx
    pop ebp
    ret

/* =========================================================================
   2. SAF ASSEMBLER _fclose ÝNÞASI (-3 LIBC FD RECOVERY)
   ========================================================================= */
_fclose:
fclose:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp + 8]  /* TCC'den gelen LIBC FD deðerini al (3-12 arasý) */
    
    /* Eðer gelen FD 3'ten küçükse (0, 1, 2 yani stdio), kapatýlamaz! */
    cmp ebx, 3
    jl .L_close_success /* Güvenle atla, iþlem yapma */

    sub ebx, 3          /* LIBC FD -> Tekrar kernel uyumlu 0-based index (0-9) */

    /* Saf TRDOS sys_close (EAX=6) çaðrýsý */
    mov eax, 6
    int 0x40
    jc .L_close_err

.L_close_success:
    xor eax, eax        /* Baþarýlý: Return 0 */
    jmp .L_fclose_done

.L_close_err:
    mov eax, -1         /* Hata: Return -1 */

.L_fclose_done:
    pop ebx
    pop ebp
    ret

/* =========================================================================
   3. ungetc MANTIK KÖPRÜSÜ (Bypass/Simülasyon Zýrhý)
   ========================================================================= */
/* arg1: int c [ebp+8], arg2: FILE *stream [ebp+12] */
// _ungetc:
// ungetc:
//    push ebp
//    mov ebp, esp
//    mov eax, [ebp + 8]   /* Geri itilmek istenen karakteri EAX'e al */
//    /* Not: Initial TCC, ungetc sonrasýnda karakteri hemen okuyacak bir */
//    /* iç tampon iþaretçisine sahiptir. Buradan karakteri güvenle geri dönüyoruz. */
//    pop ebp
//    ret

/* =========================================================================
   4. puts MANTIK KÖPRÜSÜ (Yerleþik _write ve _strlen Baðlantýsý)
   ========================================================================= */
_puts:
puts:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp + 8]   /* ebx = ekrana basýlacak dizge adresi */
    test ebx, ebx
    jz .L_puts_done

    /* Önce karakter uzunluðunu yerleþik _strlen ile bulalým */
    push ebx
    .extern _strlen
    call _strlen
    add esp, 4           /* eax = karakter sayýsý */
    test eax, eax
    jz .L_puts_lf

    /* Yerleþik _write(fd, buf, count) fonksiyonunu çaðýrýyoruz */
    push eax             /* Arg 3: count */
    push ebx             /* Arg 2: buffer pointer */
    push 1               /* Arg 1: stdout FD = 1 */
    .extern _write
    call _write
    add esp, 12

.L_puts_lf:
    /* C standardý gereði puts sonuna yeni satýr (\n) ekler */
    mov eax, offset local_lf_char
    push 1
    push eax
    push 1
    call _write
    add esp, 12

.L_puts_done:
    xor eax, eax
    pop ebx
    pop ebp
    ret

_GetProcAddress@8:
_LoadLibraryA@4:
_FreeLibrary@4:
_VirtualProtect@16:
_SetUnhandledExceptionFilter@4:
_SearchPathA@24:
    xor eax, eax
    ret

local_lf_char: .byte 10  /* LF */
