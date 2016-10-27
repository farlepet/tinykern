ENTRY (_start)

SECTIONS
{
        . = 1M;

        kern_start = .;
        
        .text BLOCK(1K) : ALIGN(1K)
        {
                *(.mboot)
                *(.text)
        }
        
        .rodata BLOCK(1K) : ALIGN(1K)
        {
                *(.rodata)
        }
        
        .data BLOCK(1K) : ALIGN(1K)
        {
                *(.data)
        }
        
        .bss BLOCK(1K) : ALIGN(1K)
        {
                *(COMMON)
                *(.bss)
        }
        
        kern_end = .;
}
