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
}