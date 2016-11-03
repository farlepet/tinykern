#include <intr/exceptions.h>
#include <mem/paging.h>
#include <io/vga/vga.h>
#include <mem/frames.h>
#include <io/serial.h>
#include <intr/idt.h>
#include <mem/gdt.h>
#include <io/vid.h>
#include <mboot.h>
#include <intr.h>

void __kernel_entry(mboot_header_t *mboot_head, u32 mboot_magic);

void __kernel_entry(mboot_header_t *mboot_head, u32 mboot_magic) {
    serial_init(SER_COM1, 38400);
    video_set_serial_port(SER_COM1);
    if(mboot_magic != 0x2BADB002) {
        klog(KLOG_FATAL, "Invalid magic number from bootloader: 0x%08X", mboot_magic);
        (void)mboot_head;
        for(;;) intr_hlt();
    }
    kputs("TinyKern built on "__DATE__" -- (c) Peter Farley\n");
    kprintf("Mem Lower: %dKiB, Upper: %dMiB\n", mboot_head->mem_lower, mboot_head->mem_upper / 1024);
    idt_init();
    exceptions_init();
    gdt_init();
    paging_init();
    frames_init(mboot_head->mem_upper * 1024);
    void *talc0 = frame_alloc_consec(4);
    kprintf("talc0: %08X\n", talc0);
    void *talc1 = frame_alloc_consec(4);
    kprintf("talc1: %08X\n", talc1);
    frame_free_consec(talc0, 4);
    void *talc2 = frame_alloc_consec(4);
    kprintf("talc2: %08X\n", talc2);
    
    __INT(0);
}
