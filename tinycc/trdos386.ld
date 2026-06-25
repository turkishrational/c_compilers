ENTRY(_start)

SECTIONS
{
    . = 0x0; /* TRDOS-386 Sanal Origin Noktasý */

    .text : {
        *crt0.o(.text)
        *(.text)
        *(.text.*)
        . = ALIGN(4096); /* Kod alanýný tam sayfa sýnýrýna kilitle */
    }

    .rodata : {
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4096);
    }

    .data : {
        *(.data)
        *(.data.*)
        . = ALIGN(4096); /* Veri segmentini tam sayfa sýnýrýna kilitle */
    }

    /* BSS, objcopy'nin diske bastýðý dosya boyutunun hemen arkasýndaki
       temiz ve hizalanmýþ ilk sayfadan baþlamak zorunda! */
    .bss : {
        _bss_start = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4096);
    }

    _end = .;
    end = .;

    /DISCARD/ : { *(.comment) *(.note.*) }

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

PROVIDE(__mingw_vfprintf = _print);
PROVIDE(___mingw_vfprintf = __print);
PROVIDE(___mingw_vfprintf = _print);

PROVIDE(__mingw_vprintf = _print);
PROVIDE(___mingw_vprintf = __print);
PROVIDE(___mingw_vprintf = _print);

}
