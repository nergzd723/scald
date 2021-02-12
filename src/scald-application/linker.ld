ENTRY(_start)
OUTPUT_FORMAT("binary")
SECTIONS {
    . = 1M;
    .text :
    {
        *(.start)
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