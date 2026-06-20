ENTRY(_start)

SECTIONS
{
    . = 0x0;

    .text : {
        /* Ne olursa olsun crt0.o dosyasýndaki .text kesitini tam olarak 0x0 adresine zorla */
        *crt0.o(.text)
        *crt0.o(.text.*)
        
        /* Geri kalan tüm .o dosyalarýnýn kod bölümleri crt0'ýn bittiði yerden devam etsin */
        *(.text)
        *(.text.*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    .data : {
        *(.data)
        *(.data.*)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }
    
    _end = .;
    end = .;

    /DISCARD/ : {
        *(.comment)
        *(.note.*)
        *(.head)
    }

/* =================================================================== */
/* TRDOS 386 LINKER ALIAS ARMOR - PHASE 2                              */
/* =================================================================== */

PROVIDE(_snprintf = _sprint);
PROVIDE(__snprintf = _sprint);
PROVIDE(snprintf = _sprint);
PROVIDE(___mingw_snprintf = _sprint);

PROVIDE(_vsnprintf = _sprint);
PROVIDE(__vsnprintf = _sprint);
PROVIDE(vsnprintf = _sprint);
PROVIDE(___mingw_vsnprintf = _sprint);

PROVIDE(_vsprintf = _sprint);
PROVIDE(__vsprintf = _sprint);
PROVIDE(vsprintf = _sprint);
PROVIDE(___mingw_vsprintf = _sprint);

PROVIDE(__mingw_vsprintf = _sprint);

PROVIDE(__mingw_vfprintf = __print);
PROVIDE(__mingw_vprintf = __print);

PROVIDE(__mingw_vfprintf = __print);
PROVIDE(__mingw_vfprintf = _print);
PROVIDE(___mingw_vfprintf = __print);
PROVIDE(___mingw_vfprintf = _print);

PROVIDE(__mingw_vprintf = __print);
PROVIDE(__mingw_vprintf = _print);
PROVIDE(___mingw_vprintf = __print);
PROVIDE(___mingw_vprintf = _print);

}