.intel_syntax noprefix

.global _malloc
.global malloc

/* 24/07/2026 */
.global _trdos_free

.text

.align 4
/* Bir sonraki malloc tahsisatý için statik hafýza takip hücresi */
current_break_storage: .long 0

/* 26/6/2026 - Google AI */
/* =========================================================================
   TRDOS 386 SAF FLAT MALLOC MOTORU (EAX=[u.r0] ENTEGRASYONU)
   ========================================================================= */
_malloc:
malloc:
    push ebp
    mov ebp, esp
    push ebx
    push ecx

    mov ecx, [ebp + 8]          /* ecx = Ýstenen bellek boyutu (size) */
    test ecx, ecx
    jz .L_malloc_failed         /* Eðer size 0 ise doðrudan NULL (0) dön */

    /* 4-Byte Dword Hizalama Zýrhý: (size + 3) & ~3 */
    add ecx, 3
    and ecx, 0xFFFFFFFC

    /* Statik bellek takibimizi sorguluyoruz */
    mov ebx, [current_break_storage]
    test ebx, ebx
    jnz .L_calc_new_break       /* Eðer daha önce u.break alýnmýþsa doðrudan hesapla */

    /* =========================================================================
       AÞAMA A: ÝLK ÇAÐRIDA KERNEL'DAN BAÞLANGIÇ BSS (u.break) ADRESÝNÝ ALMA
       ========================================================================= */
    // mov ebx, -1              /* EBX = -1 -> Mevcut u.break adresini ver */
    dec ebx                     /* -1 */
    mov eax, 17                 /* EAX = 17 (sys_break) */
    int 0x40                    /* Çekirdek Kesmesi */
    jc .L_malloc_failed         /* Hata varsa (CF=1) elenir */

    /* Kernel'dan gelen [u.r0] (current_break) adresini 4-byte'a hizalayýp depola */
    add eax, 3
    and eax, 0xFFFFFFFC
    mov [current_break_storage], eax

    mov ebx, eax

.L_calc_new_break:
    /* ebx = current_break, ecx = aligned_size */
    add ebx, ecx                /* ebx = new_break adresi (current_break + size) */

    /* =========================================================================
       AÞAMA B: KERNEL'DA u.break ALANINI GENÝÞLETME (ZERO-FILL TETÝKLEMESÝ)
       ========================================================================= */
    mov eax, 17                 /* EAX = 17 (sys_break) */
    int 0x40                    /* Çekirdek Kesmesi */
    jc .L_malloc_failed         /* Hata varsa (CF=1) elenir, EAX=0 olur */

    /* =========================================================================
       AÞAMA C: STATÝK GÜNCELLEME VE ESKÝ ADRESÝ GERÝ DÖNME DÖNGÜSÜ
       ========================================================================= */
    /* Kesme baþarýyla döndü (CF=0). [u.r0] uyarýnca EAX = new_break adresidir! */

    /* Bir sonraki malloc çaðrýsý için depomuzu kernel'ýn döndüðü new_break (EAX) ile güncelliyoruz */
    xchg eax, [current_break_storage]

    /* Return EAX = Tahsis edilen alanýn baþlangýç adresi (eski break) */
    jmp .L_malloc_done

.L_malloc_failed:
    xor eax, eax                /* Hata veya yetersiz bellek: Return NULL (0) */

.L_malloc_done:
    pop ecx
    pop ebx
    pop ebp
    ret

_trdos_free:
    xor eax, eax
    ret
