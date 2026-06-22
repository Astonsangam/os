ENTRY(_loader)
/* OUTPUT_FORMAT(elf32-i386) */
/* OUTPUT_ARCH(i386:i386) */

SECTIONS
{
    . = 0x100000; /* Load kernel at 1MB */

    .text :
    {
        *(.multiboot)
        *(.text)
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
        *(COMMON)
    }

    /DISCARD/ :
    {
        *(.fini_array*)
        *(.comment)
        *(.reloc)
    }
}
