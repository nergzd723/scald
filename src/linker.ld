ENTRY(boot)
SECTIONS {
    . = 0x7c00;
    .text :
    {
        *(.boot)
        *(.text)
        *(.end)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}