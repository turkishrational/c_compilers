/* link.ld */
OUTPUT_FORMAT("binary") /* Doðrudan PRG (flat binary) üretir */
ENTRY(__start)          /* crt0.asm içindeki giriþ noktasý */

SECTIONS
{
    . = 0x0;            /* org 0x0 */

    .text : {
        crt0.o(.text)   /* Önce crt0 gelsin (jmp START için) */
        *(.text)
    }

    .data : {
        *(.data)
        *(.rodata)
    }

    .bss : {
        *(.bss)
        *(COMMON)
        _end = .;       /* sbrk için bss sonu */
    }
}
