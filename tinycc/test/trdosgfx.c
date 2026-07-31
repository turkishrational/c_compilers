/* 22/07/2026 */
/* 21/07/2026 */
/* 20/07/2026 - Google AI - ref: aclock5.s - Erdogan Tan */
/* =========================================================================
   trdos_gfx.c - TRDOS 386 Yerel Grafik ve Sistem Surucu Motoru
   ========================================================================= */
#include "trdosgfx.h"

unsigned int _x0 = 512, _y0 = 384, radius = 200;
unsigned char color = 0x0B;
unsigned int angle = 0;
unsigned int circlebuffer[40000];      /* Akselerasyon piksel havuzu */
unsigned int *pixelpos = circlebuffer; /* Imlec baslangic atamasi */

/* 181 Elemanli Sabit Noktali Sinus Tablosu (0 - 180 Derece) */
const unsigned int sinustable[181] = {
    0, 292803, 585516, 878052, 1170319, 1462231, 1753697, 2044628, 2334937, 2624535, 2913333,
    3201244, 3488179, 3774052, 4058776, 4342263, 4624427, 4905183, 5184445, 5462127, 5738146,
    6012416, 6284856, 6555381, 6823909, 7090358, 7354647, 7616697, 7876426, 8133756,
    /* 30 Derece */
    8388608, 8640905, 8890570, 9137527, 9381700, 9623016, 9861400, 10096781, 10329086, 10558244,
    10784187, 11006844, 11226149, 11442034, 11654434,
    /* 45 Derece */
    11863283, 12068519, 12270079, 12467901, 12661926, 12852093, 13038346, 13220627, 13398880, 13573053,
    13743091, 13908942, 14070557, 14227886, 14380881,
    /* 60 Derece */
    14529495, 14673684, 14813402, 14948609, 15079262, 15205322, 15326749, 15443509, 1555564, 15662880,
    15765426, 15863169, 15956081, 16044131, 16127295, 16205546, 16278861, 16347217, 16410594, 16468971,
    16522332, 16570661, 16613941, 16652161, 16685309, 16713374, 16736348, 16754223, 16766996, 16774661,
    /* 90 Derece */
    16777216, 16774661, 16766996, 16754223, 16736348, 16713374, 16685309, 16652161, 16613941, 16570661,
    16522332, 16468971, 16410594, 16347217, 16278861, 16205546, 16127295, 16044131, 15956081, 15863169,
    15765426, 15662880, 15555564, 15443509, 15326749, 15205322, 15079262, 14948609, 14813402, 14673684,
    /* 120 Derece */
    14529495, 14380881, 14227886, 14070557, 13908942, 13743091, 13573053, 13398880, 13220627, 13038346,
    12852093, 12661926, 12467901, 12270079, 12068519,
    /* 135 Derece */
    11863283, 11654434, 11442034, 11226149, 11006844, 10784187, 10558244, 10329086, 10096781, 9861400,
    9623016, 9381700, 9137527, 8890570, 8640905,
    /* 150 Derece */
    8388608, 8133756, 7876426, 7616697, 7354647, 7090358, 6823909, 6555381, 6284856, 6012416, 5738146,
    5462127, 5184445, 4905183, 4624427, 4342263, 4058776, 3774052, 3488179, 3201244, 2913333, 2624535,
    2334937, 2044628, 1753697, 1462231, 1170319, 878052, 585516, 292803,
    /* 180 Derece */
    0
};

/* Merkez Beyaz Nokta Matrisi */
const unsigned int centermark[12] = {
    (1024*382)+510, (1024*382)+511,
    (1024*383)+509, (1024*383)+510, (1024*383)+511, (1024*383)+512,
    (1024*384)+509, (1024*384)+510, (1024*384)+511, (1024*384)+512,
    (1024*385)+510, (1024*385)+511
};

/* =========================================================================
   TRDOS 386 SAF TCC INLINE ASM SURUCU METODLARI (0.9.18 / 0.9.27 UYUMLU)
   ========================================================================= */

void trdos_print_msg(const char *msg) {
    __asm__(
        "movl %0, %%ebx\n\t"    /* msg isaretcisini ebx'e at */
        "movl $255, %%ecx\n\t"  /* Tampon boyutu */
        "movl $7, %%edx\n\t"    /* Renk niteligi */
        "movl $35, %%eax\n\t"   /* _msg sistem cagrisi */
        "int $0x40"             /* Ring 0 Kesmesi */
        :
        : "r" (msg)
        : "eax", "ebx", "ecx", "edx"
    );
}

int trdos_set_video_mode(unsigned int mode_func, unsigned int mode_val) {
    int res;
    __asm__(
        "movl %1, %%ebx\n\t"    /* mode_func -> ebx */
        "movl %2, %%ecx\n\t"    /* mode_val -> ecx */
        "movl $31, %%eax\n\t"   /* _video sistem cagrisi */
        "int $0x40\n\t"
        "movl %%eax, %0"        /* Donen sonucu res degiskenine yaz */
        : "=r" (res)
        : "g" (mode_func), "g" (mode_val)
        : "eax", "ebx", "ecx"
    );
    return res;
}

void trdos_get_time(TRDOSTime *t) {
    unsigned int raw;
    __asm__(
        "movl $1, %%ebx\n\t"    /* Sub-function 1 */
        "movl $13, %%eax\n\t"   /* _time sistem cagrisi */
        "int $0x40\n\t"
        "movl %%eax, %0"        /* EAX -> raw */
        : "=r" (raw)
        :
        : "eax", "ebx"
    );
    if (t) {
        t->second = raw & 0xFF;
        t->minute = (raw >> 8) & 0xFF;
        t->hour   = (raw >> 16) & 0xFF;
    }
}

unsigned int trdos_get_ticks(void) {
    unsigned int ticks;
    __asm__(
        "movl $4, %%ebx\n\t"    /* Sub-function 4 */
        "movl $13, %%eax\n\t"   /* _time sistem cagrisi */
        "int $0x40\n\t"
        "movl %%eax, %0"        /* EAX -> ticks */
        : "=r" (ticks)
        :
        : "eax", "ebx"
    );
    return ticks;
}

int trdos_getchar_nowait(void) {
    int key;
    __asm__(
        "movl $1, %%ebx\n\t"    /* Sub-function 1 */
        "movl $46, %%eax\n\t"   /* _stdio sistem cagrisi */
        "int $0x40\n\t"
        "movl %%eax, %0"        /* EAX -> key */
        : "=r" (key)
        :
        : "eax", "ebx"
    );
    return key;
}

void trdos_beep(unsigned int duration, unsigned int freq) {
    __asm__(
        "movl %0, %%ebx\n\t"    /* duration -> ebx */
        "movl %1, %%ecx\n\t"    /* freq -> ecx */
        "movl $32, %%eax\n\t"   /* _audio sistem cagrisi */
        "int $0x40"
        :
        : "g" (duration), "g" (freq)
        : "eax", "ebx", "ecx"
    );
}

void trdos_write_buffer(unsigned int count, unsigned char color_val) {
    /* circlebuffer dizisinin baslangic adresini (isaretcisini) aliyoruz */
    unsigned int *ptr = (unsigned int *)&circlebuffer;

    __asm__(
        "movl $0x0305, %%ebx\n\t"  /* Sub-function 0305h */
        "movl %0, %%ecx\n\t"       /* color_val -> ecx */
        "movl %1, %%edx\n\t"       /* count -> edx */
        "movl %2, %%esi\n\t"       /* ptr icindeki gercek tampon bellek adresi -> esi */
        "movl $31, %%eax\n\t"      /* _video sistem cagrisi */
        "int $0x40"                /* UYUM KILIDI: TCC icin satir ici AT&T kesme formati */
        :
        : "g" ((unsigned int)color_val), "g" (count), "r" (ptr)
        : "eax", "ebx", "ecx", "edx", "esi"
    );
}

void draw_centermark(void) {
    unsigned int *ptr = (unsigned int *)&centermark;

    __asm__(
        "movl $0x0305, %%ebx\n\t"
        "movl $0x0F, %%ecx\n\t"
        "movl $12, %%edx\n\t"
        "movl %0, %%esi\n\t"
        "movl $31, %%eax\n\t"
        "int $0x40"
        :
        : "r" (ptr)
        : "eax", "ebx", "ecx", "edx", "esi"
    );
}

/* 21/07/2026 - Google AI */
/* =========================================================================
   TRDOS 386 FLAT BRESENHAM CIZGI CIZME MOTORU (draw_line)
   ========================================================================= */
void draw_line(unsigned int target_angle) {
    unsigned int calc_y;
    unsigned int offset;
    unsigned int prev_val;
    unsigned int count = 0;

    angle = target_angle;
    
    /* 1. Aciya gore sabit noktali trigonometrik izdusumleri hesapla */
    unsigned int cos_val = getcosinus(angle);
    unsigned int _x1 = getxy(cos_val);
    
    unsigned int sin_val = getsinus(angle);
    unsigned int _y1 = getxy(sin_val);

    /* Piksel havuz (circlebuffer) yazim imlecini sifirla */
    pixelpos = (unsigned int *)&circlebuffer;
    unsigned int _x2 = 0;
    unsigned int _y2 = 0;

    /* 2. Yardimci Piksel Konumlandirici Makro Govdesi (write_line_pixel) */
    #define WRITE_LINE_PIXEL(cur_x, cur_y) { \
        calc_y = _y0; \
        if (angle <= 180) { \
            calc_y -= cur_y; \
        } else { \
            calc_y += cur_y; \
        } \
        offset = (calc_y * 1024) + _x0; \
        if (angle <= 90 || angle > 270) { \
            offset += cur_x; \
        } else { \
            offset -= cur_x; \
        } \
        *pixelpos = offset; \
        pixelpos++; \
        count++; \
    }

    /* 3. Ana Eksen Cizim Algoritmasi (0.9.18 & 0.9.27 __udivdi3 ENGELLEYICI) */
    if (_x1 >= _y1) {
        /* X ekseni tabanli yuruyus hatti */
        WRITE_LINE_PIXEL(_x2, _y2);
        prev_val = _y2;
        while (_x2 < _x1) {
            _x2++;
            
            /* TCC'nin 64-bit bolme fonksiyonunu (__udivdi3) cagirmasini el ile engelliyoruz:
               EAX = _x2, sin_val ile carpilir -> Sonuc EDX:EAX (64-bit) olur.
               Ardindan dogrudan cos_val'e bolunur, sonuc EAX'e (_y2) oturur. */
            __asm__(
                "movl %1, %%eax\n\t"
                "mull %2\n\t"          /* EDX:EAX = _x2 * sin_val */
                "divl %3\n\t"          /* EAX = EDX:EAX / cos_val */
                "movl %%eax, %0"
                : "=r" (_y2)
                : "g" (_x2), "g" (sin_val), "g" (cos_val)
                : "eax", "edx"
            );

            prev_val++;
            if (prev_val < _y2) {
                _x2--;
                while (prev_val < _y2) {
                    _y2 = prev_val;
                    WRITE_LINE_PIXEL(_x2, _y2);
                    prev_val++;
                }
                _x2++;
            }
            WRITE_LINE_PIXEL(_x2, _y2);
        }
    } else {
        /* Y ekseni tabanli yuruyus hatti */
        WRITE_LINE_PIXEL(_x2, _y2);
        prev_val = _x2;
        while (_y2 < _y1) {
            _y2++;
            
            /* Ayni donanimsal akselerasyon Y ekseni icin de gecerli:
               EDX:EAX = _y2 * cos_val -> divl sin_val -> Sonuc EAX'e (_x2) oturur. */
            __asm__(
                "movl %1, %%eax\n\t"
                "mull %2\n\t"          /* EDX:EAX = _y2 * cos_val */
                "divl %3\n\t"          /* EAX = EDX:EAX / sin_val */
                "movl %%eax, %0"
                : "=r" (_x2)
                : "g" (_y2), "g" (cos_val), "g" (sin_val)
                : "eax", "edx"
            );

            prev_val++;
            if (prev_val < _x2) {
                _y2--;
                while (prev_val < _x2) {
                    _x2 = prev_val;
                    WRITE_LINE_PIXEL(_x2, _y2);
                    prev_val++;
                }
                _y2++;
            }
            WRITE_LINE_PIXEL(_x2, _y2);
        }
    }
    #undef WRITE_LINE_PIXEL

    /* 4. Havuzda toplanan pikselleri tek hamlede Ring 0 VESA surucusune gonder */
    if (count > 0) {
        trdos_write_buffer(count, color);
    }
}

unsigned int getsinus(unsigned int angle_deg) {
    /* 180 derece veya uzerini simetrik olarak suz */
    if (angle_deg >= 180) {
        angle_deg -= 180;
    }
    if (angle_deg > 180) return 0; /* Guvenlik siniri */
    return sinustable[angle_deg];
}

unsigned int getcosinus(unsigned int angle_deg) {
    /* Kosinus, sinusun 90 derece otelenmis halidir */
    unsigned int target = angle_deg + 90;
    if (target >= 360) {
        target -= 360;
    }
    return getsinus(target);
}

/* =========================================================================
   TRDOS 386 SAF 32-BIT HARDWARE PROJECTION MOTOR (getxy)
   ========================================================================= */
unsigned int getxy(unsigned int trig_value) {
    unsigned int result;

    /* Intel 386 donanim carpim mekanizmasini el ile muhurluyoruz:
       EAX = trig_value, MUL radius komutu -> Sonucu EDX:EAX yapar.
       Ardindan EDX:EAX ciftini 24 bit saga kaydirip sonucu result'a aliriz. */
    __asm__(
        "movl %1, %%eax\n\t"    /* trig_value -> EAX */
        "mull %2\n\t"           /* EAX * radius -> ust 32-bit EDX'e, alt 32-bit EAX'e */
        "shrdl $24, %%edx, %%eax\n\t" /* EDX:EAX ciftini 24 bit saga kaydir, sonuc EAX'e oturur */
        "movl %%eax, %0"        /* EAX -> result */
        : "=r" (result)
        : "g" (trig_value), "g" (radius)
        : "eax", "edx"
    );

    return result;
}

/* 21/07/2026 - Google AI */
/* =========================================================================
   TRDOS 386 HARDWARE ACCELERATED CLOCK BACKGROUND GENERATOR (draw_background)
   ========================================================================= */
void draw_background(void) {
    unsigned int temp_angle;
    unsigned int count;

    /* 1. Merkez Koordinatlari ve Yaricapi ASM Standartlarina Gore Kilitle */
    _x0 = 1024 / 2;      /* Ekran genisliginin tam ortasi */
    _y0 = 768 / 2;       /* Ekran yuksekliginin tam ortasi */
    radius = 200;        /* Ana dis cember yaricapi */

    /* 2. Dis cemberi Turkuaz (Cyan) Renkle ciz */
    color = 0x0B;        /* Cyan renk kodu */
    draw_circle();       /* Pikselleri hazirlar ve tek seferde sys_video ile basar */

    /* =====================================================================
       3. DAKIKA NOKTALARINI HESAPLA VE CIZ (draw_minute_dots)
       ===================================================================== */
    radius = 188;        /* Noktalarin cembere gore ic yaricapi */
    count = 0;
    pixelpos = circlebuffer; /* Havuz yazim imlecini sifirla */

    /* 0 dereceden 360 dereceye 6'sar derece adimlarla 60 dakikayi tara */
    for (temp_angle = 0; temp_angle < 360; temp_angle += 6) {
        /* Aciya gore sabit noktali trigonometrik izdusumleri al */
        unsigned int cos_v = getcosinus(temp_angle);
        unsigned int _x1 = getxy(cos_v);

        unsigned int sin_v = getsinus(temp_angle);
        unsigned int _y1 = getxy(sin_v);

        /* Aci bolgesine (Phase/Quarter) gore ekran offsetini hesapla */
        unsigned char current_phase = 0;
        if (temp_angle <= 90) current_phase = 0;
        else if (temp_angle <= 180) current_phase = 1;
        else if (temp_angle <= 270) current_phase = 2;
        else current_phase = 3;

        unsigned int calc_y = _y0;
        unsigned int offset;

        if (current_phase == 0) { calc_y -= _y1; offset = (calc_y * 1024) + (_x0 + _x1); }
        else if (current_phase == 1) { calc_y -= _y1; offset = (calc_y * 1024) + (_x0 - _x1); }
        else if (current_phase == 2) { calc_y += _y1; offset = (calc_y * 1024) + (_x0 - _x1); }
        else { calc_y += _y1; offset = (calc_y * 1024) + (_x0 + _x1); }

        /* Pikseli akselerasyon havuzuna kaydet */
        *pixelpos = offset;
        pixelpos++;
        count++;
    }
    
    /* Dakika noktalarini tek hamlede Ring 0 VESA surucusune gonder */
    if (count > 0) {
        trdos_write_buffer(count, 0x0B); /* Turkuaz noktalar */
    }

    /* =====================================================================
       4. SAAT (5 DAKIKALIK) KARELERINI HESAPLA VE CIZ (draw_hour_squares)
       ===================================================================== */
    radius = 188;
    
    /* 0 dereceden 360 dereceye 30'ar derece adimlarla 12 saati tara */
    for (temp_angle = 0; temp_angle < 360; temp_angle += 30) {
        count = 0;
        pixelpos = circlebuffer; /* Her saat karesi icin havuzu tazele */

        /* ASM'deki dhs_2 ve dhs_1 ic ice kaydirma dongulerinin C karsiligi:
           Her saat gostergesi icin -2'den +2'ye 5x5 piksel boyutunda kalin bir kare cizer */
        int offset_x, offset_y;
        for (offset_y = -2; offset_y <= 2; offset_y++) {
            for (offset_x = -2; offset_x <= 2; offset_x++) {
                
                /* Merkez orijini dinamik olarak kaydiriyoruz */
                unsigned int local_x0 = (1024 / 2) + offset_x;
                unsigned int local_y0 = (768 / 2) + offset_y;

                unsigned int cos_v = getcosinus(temp_angle);
                unsigned int _x1 = getxy(cos_v);

                unsigned int sin_v = getsinus(temp_angle);
                unsigned int _y1 = getxy(sin_v);

                unsigned char current_phase = 0;
                if (temp_angle <= 90) current_phase = 0;
                else if (temp_angle <= 180) current_phase = 1;
                else if (temp_angle <= 270) current_phase = 2;
                else current_phase = 3;

                unsigned int calc_y = local_y0;
                unsigned int offset;

                if (current_phase == 0) { calc_y -= _y1; offset = (calc_y * 1024) + (local_x0 + _x1); }
                else if (current_phase == 1) { calc_y -= _y1; offset = (calc_y * 1024) + (local_x0 - _x1); }
                else if (current_phase == 2) { calc_y += _y1; offset = (calc_y * 1024) + (local_x0 - _x1); }
                else { calc_y += _y1; offset = (calc_y * 1024) + (local_x0 + _x1); }

                *pixelpos = offset;
                pixelpos++;
                count++;
            }
        }

        /* Saat karesini tek hamlede Ring 0 VESA surucusune gonder */
        if (count > 0) {
            trdos_write_buffer(count, 0x0B); /* Kalin saat gostergeleri */
        }
    }
}

/* 21/07/2026 - Google AI */
/* =========================================================================
   TRDOS 386 INDEPENDENT HARDWARE ACCELERATED CIRCLE MOTOR (draw_circle)
   ========================================================================= */
/* x^2 + y^2 = r^2 formuluyle pikselleri birbirine degecek sekilde (kesintisiz dolguyla) dizer. */

void draw_circle(void) {
    int cur_x;
    int cur_y;
    int prev_y;
    int fill_y;
    int calc_y;
    unsigned int offset;
    unsigned int count = 0;

    /* Piksel havuz (circlebuffer) yazim imlecini en basa muhurle */
    pixelpos = circlebuffer;

    /* Saf 4 Yonlu Kartezyen Grafik Offset Muhurleyicisi */
    #define PLOT_4_WAY_SYMMETRY(X, Y) { \
        calc_y = (int)_y0 - Y; offset = (calc_y * 1024) + ((int)_x0 + X); *pixelpos = offset; pixelpos++; count++; \
        calc_y = (int)_y0 - Y; offset = (calc_y * 1024) + ((int)_x0 - X); *pixelpos = offset; pixelpos++; count++; \
        calc_y = (int)_y0 + Y; offset = (calc_y * 1024) + ((int)_x0 - X); *pixelpos = offset; pixelpos++; count++; \
        calc_y = (int)_y0 + Y; offset = (calc_y * 1024) + ((int)_x0 + X); *pixelpos = offset; pixelpos++; count++; \
    }

    /* Initial state: x = 200 iken y = 0 */
    cur_y = 0;
    prev_y = 0;

    /* =====================================================================
       TRDOS 386 - ERDOGAN TAN KESINTISIZ TAM CEMBER DONGUSU
       =====================================================================
       cur_x bagimsiz degiskeni tipki ASM'deki gibi 200'den 0'a geriye dogru azalir.
       Tamsayi aritmetiginin engellerini asmak ve yanlardaki basikligi engellemek icin
       (cur_y * cur_y) + (cur_x * cur_x) dengesi tavan tam sayiya yuvarlanarak izlenir. */
    for (cur_x = 200; cur_x >= 0; cur_x--) {
        
        /* x86 Tamsayi Diferansiyel Esitlemesi:
           Karelerin toplami yaricapin karesini (40000) gecene kadar cur_y'yi artiriyoruz.
           Bu mantik, sizin ASM arama motorundaki tavan tamsayi (ceiling) karakteristigini
           hicbir Look-Up tablosuna ihtiyac duymadan run-time'da %100 dogrular. */
        while ((cur_y * cur_y) + (cur_x * cur_x) < (200 * 200)) {
            cur_y++;
        }

        /* DIKEY DOLGU: AutoCAD tarzi kesintisiz Orgu hatti.
           Iki adim arasinda ordinat sicramasi (bosluk) varsa, dikey eksende 
           pikseller birbirine milimetrik degene kadar arayi tam kapat. */
        if (cur_y > prev_y && cur_x < 200) {
            for (fill_y = prev_y + 1; fill_y < cur_y; fill_y++) {
                PLOT_4_WAY_SYMMETRY(cur_x, fill_y);
            }
        }

        /* Ana koordinat offset ciftini dort ceyrege birden muhurle */
        PLOT_4_WAY_SYMMETRY(cur_x, cur_y);
        
        /* Bir sonraki adim karsilastirmasi icin y degerini kaydet */
        prev_y = cur_y;
    }

    /* cemberin tepe ve alt birlesme sinirindaki (x=0) son dikey boslugu kapat */
    for (fill_y = prev_y; fill_y <= 200; fill_y++) {
        PLOT_4_WAY_SYMMETRY(0, fill_y);
    }

    #undef PLOT_4_WAY_SYMMETRY

    /* Hazirlanan yasal dword offset dizisini tek hamlede Ring 0 VESA surucusune gonder */
    if (count > 0) {
        trdos_write_buffer(count, color);
    }
}

/* 22/07/2026 - Google AI */
/* =========================================================================
   TRDOS 386 SAF DONANIM EXIT KOPRUSU (trdos_sysexit)
   =========================================================================
   main fonksiyonu bittiginde stack kaymasini (EIP=24h) engelleyerek 
   dogrudan Ring 0 sys_exit cagrisi yapar. */
void trdos_sysexit(void) {
    __asm__ __volatile__ (
        "movl $1, %ebx\n\t"   /* Exit payload: 1 */
        "movl $1, %eax\n\t"   /* sys_exit Servis Indeksi */
        "int $0x40"           /* Dogrudan Ring 0'a firlat, terminale don */
    );
}

