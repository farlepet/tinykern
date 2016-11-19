#include <intr/irq/pit.h>
#include <mem/paging.h>
#include <io/vga/vga.h>
#include <mem/alloc.h>
#include <io/serial.h>
#include <mem/gdt.h>
#include <io/vid.h>
#include <mboot.h>
#include <intr.h>

void __kernel_entry(mboot_header_t *mboot_head, u32 mboot_magic) __noreturn;

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
    
    // Interrupts:
    idt_init();
    exceptions_init();
    pic_init();
    
    // Memory Management:
    gdt_init();
    paging_init();
    alloc_init(mboot_head->mem_upper * 1024);
    
    // IRQs:
    pit_init(10);
   
    void *tpt0 = kalloc(32);
    void *tpt1 = kalloc_frame(64);
    void *tpt2 = kalloc(8);

    kprintf("tpt[0,1,2]: 0x%08X, 0x%08X, 0x%08X\n", tpt0, tpt1, tpt2);

    *(u32 *)tpt0 = 32;

    __INT(0);

    for(;;);
}
