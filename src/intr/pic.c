#include <io/ioport.h>
#include <intr/pic.h>
#include <io/vid.h>
#include <intr.h>

void pic_init() {
    intr_disable(); // Disable interrupts

    // Start Initialization
    outb(PIC1_COM, ICW1_ICW4 | ICW1_INIT);
    outb(PIC2_COM, ICW1_ICW4 | ICW1_INIT);

    // Set offsets
    outb(PIC1_DATA, PIC1_OFF);
    outb(PIC2_DATA, PIC2_OFF);

    // Tell PIC's how they are connected
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Set 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // Disable all IRQ's
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);

    intr_enable(); // Enable interrupts again
}

void set_irq_handler(u8 irq, u32 handler) {
   idt_create_entry(&IDT[PIC1_OFF + irq], handler, 0x08,  IDT_FLAG_TYPE_INTR32 | IDT_FLAG_PRIV_RING0 | IDT_FLAG_PRESENT);
}

void irq_disable(u8 irq) {
    if(irq <= 7) {
        outb(PIC1_DATA, inb(PIC1_DATA) | (u8)(1 << irq));
    } else if(irq <= 14) {
        outb(PIC2_DATA, inb(PIC2_DATA) | (u8)(1 << (irq - 7)));
    } else {
        klog(KLOG_ERR, "irq_disable: Invalid IRQ number: %d", irq);
    }
}

void irq_enable(u8 irq) {
    if(irq <= 7) {
        outb(PIC1_DATA, inb(PIC1_DATA) & (u8)~(1 << irq));
    } else if(irq <= 14) {
        outb(PIC2_DATA, inb(PIC2_DATA) & (u8)~(1 << (irq - 7)));
    } else {
        klog(KLOG_ERR, "irq_enable: Invalid IRQ number: %d", irq);
    }
}
