.intel_syntax noprefix
.global _freopen
.global freopen
.text

_freopen:
freopen:
    push ebp
    mov ebp, esp
    push ebx

    /* =========================================================================
       1. KISIM: MEVCUT DISK DOSYASINI KAPAT (sys _close)
       ========================================================================= */
    mov ebx, [ebp + 16]   /* ebx = TCC'den gelen LIBC FD deðeri */
    cmp ebx, 3
    jl .L_skip_close      /* Eðer 0, 1, 2 (stdin/out/err) ise KAPATMA! */

    sub ebx, 3            /* LIBC FD -> TRDOS FD dönüþümü */
    mov eax, 6            /* _close equ 6 */
    int 0x40              /* TRDOS Kernel Kesmesi */

.L_skip_close:
    /* =========================================================================
       2. KISIM: YENÝ DOSYAYI AÇ (sys _open)
       ========================================================================= */
    mov ecx, [ebp + 12]   /* ecx = mode string adresi */
    mov cl, [ecx]

    xor ecx, ecx          /* Varsayýlan: O_RDONLY (0) */
    cmp cl, 119           /* 'w' karakteri mi? */
    jne .L_check_append
    mov ecx, 1            /* O_WRONLY (1) */
    jmp .L_do_open

.L_check_append:
    cmp cl, 97            /* 'a' karakteri mi? */
    jne .L_do_open
    mov ecx, 2            /* O_RDWR (2) */

.L_do_open:
    mov ebx, [ebp + 8]    /* ebx = pathname */
    xor edx, edx          /* edx = mode (0) */
    mov eax, 5            /* _open equ 5 */
    int 0x40
    jc .L_fail

    add eax, 3            /* TRDOS FD -> LIBC FD dönüþümü (+3 zýrhý) */
    jmp .L_final_done

.L_fail:
    mov eax, 0            /* Hata anýnda C standardý NULL döner */

.L_final_done:
    pop ebx
    pop ebp
    ret

