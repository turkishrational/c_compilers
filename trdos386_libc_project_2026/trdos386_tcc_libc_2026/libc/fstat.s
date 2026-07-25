.intel_syntax noprefix
.global _fstat
.global fstat
.text

_fstat:
fstat:
    push ebp
    mov ebp, esp
    push ebx              /* cdecl korumasý */

    mov ebx, [ebp + 8]    /* Argument 1: LIBC FD (3-12 arasý) */

    mov ecx, [ebp + 12]   /* Argument 2: struct stat *buf bellek adresi */

    /* ÖNEMLÝ: Standart akýþlar (0,1,2) için fstat istenirse disk sorgusu yapma, nötr dön */
    cmp ebx, 3
    jl .L_fstat_stub

    sub ebx, 3            /* LIBC FD -> TRDOS FD (0-9) dönüþümü */

    mov eax, 47           /* _fstat equ 47 (TRDOS 386 v2.0.11 Kernel API) */
    int 0x40              /* Çekirdek Kesmesi */

.L_fstat_done:
    pop ebx
    pop ebp
    ret

.L_fstat_stub:
    /* stdin/out/err için sahte tampon doldurma (çökmeyi önleme zýrhý) */
    xor eax, eax
    mov [ecx], eax        /* st_dev = 0 */
    mov [ecx + 20], eax   /* st_size = 0 */
    jmp .L_fstat_done


