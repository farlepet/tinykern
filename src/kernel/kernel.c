#include <mem/paging.h>
#include <io/vga/vga.h>
#include <io/serial.h>
#include <mem/gdt.h>
#include <io/vid.h>

void __kernel_entry(void);

void __kernel_entry(void) {
    serial_init(SER_COM1, 38400);
    video_set_serial_port(SER_COM1);
    gdt_init();
    kputs("TinyKern built on "__DATE__" -- (c) Peter Farley\n");
    paging_init();
    kprintf("TEST: %+d, %d, %d, %d\n", 32, 0xDEAD, 0x8000DEAD, 0x1);
}
