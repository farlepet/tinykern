#include <intr/exceptions.h>
#include <mem/paging.h>
#include <io/vga/vga.h>
#include <io/serial.h>
#include <intr/idt.h>
#include <mem/gdt.h>
#include <io/vid.h>
#include <intr.h>

void __kernel_entry(void);

void __kernel_entry(void) {
    serial_init(SER_COM1, 38400);
    video_set_serial_port(SER_COM1);
    idt_init();
    exceptions_init();
    gdt_init();
    kputs("TinyKern built on "__DATE__" -- (c) Peter Farley\n");
    paging_init();
    
    __INT(0);
}
