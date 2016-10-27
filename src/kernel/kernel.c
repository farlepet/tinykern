#include <io/vga/vga.h>
#include <io/vid.h>

void __kernel_entry(void);

void __kernel_entry(void) {
    vga_clear();
    kputs("TinyKern built on "__DATE__" -- (c) Peter Farley\n");
    kprintf("TEST: %+d, %d, %d, %d\n", 32, 0xDEAD, 0x8000DEAD, 0x1);
}
