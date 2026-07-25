int main() {
    /* 1. Adım: Standart C Hesaplama Aşaması */
    int a = 10;
    int b = 20;
    int c = a + b;

    /* 2. Adım: TRDOS İletisi İçin Saf Bellek Alanı */
    /* İletiyi statik yapıyoruz ki hafızadaki yeri (data section) sabit olsun */
    static const char msg_text[] = "\r\n-> [TRDOS NATIVE TCC]: Flat PRG execution test is successful!\r\n\0";

    /* 3. Adım: TCC'nin Sadece Ham Veri Olarak Göreceği Saf .byte Enjeksiyonu */
    /* Hiçbir iki nokta üst üste (:), kısıtlama veya talimat ismi içermez! */
    __asm__ __volatile__ (
        ".byte 0xBB\n\t" /* mov ebx, msg_text (Aşağıdaki işaretçiyi ebx'e yükler) */
        ".long msg_text\n\t"
        ".byte 0xB9, 0x41, 0x00, 0x00, 0x00\n\t" /* mov ecx, 65 (İleti uzunluğu) */
        ".byte 0xBA, 0x0B, 0x00, 0x00, 0x00\n\t" /* mov edx, 0x0B (Açık Mavi renk) */
        ".byte 0xB8, 0x23, 0x00, 0x00, 0x00\n\t" /* mov eax, 35 (_msg sistem çağrısı) */
        ".byte 0xCD, 0x40\n"                     /* int 0x40 (TRDOS Kernel Kesmesi) */
    );

    /* TRDOS exit code dönüşü */
    return c;
}



