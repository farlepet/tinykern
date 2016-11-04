#ifndef INTR_PIC_H
#define INTR_PIC_H

#include <io/ioport.h>

enum pic_ports {
    PIC1_COM  = 0x20, // PIC 1 command
    PIC2_COM  = 0xA0, // PIC 2 command
    PIC1_DATA = 0x21, // PIC 1 data
    PIC2_DATA = 0xA1  // PIC 2 data
};

#define PIC1_OFF 0x70
#define PIC2_OFF (PIC1_OFF + 0x08)

enum pic_cmd {
    ICW1_ICW4       = 0x01, // ICW4 Needed
    ICW1_SINGLE     = 0x02, // Single (cascade) mode
    ICW1_INTERVAL4  = 0x04, // Call Address interval 4 (8)
    ICW1_LEVEL      = 0x08, // Level triggered (edge) mode
    ICW1_INIT       = 0x10, // Initialization

    ICW4_8086       = 0x01, // 8086/88 (MCS-80/85) mode
    ICW4_AUTO       = 0x02, // Auto (normal) EOI
    ICW4_BUF_SLAVE  = 0x04, // Buffered mode/slave
    ICW4_BUF_MASTER = 0x08, // Buffered mode/master
    ICW4_SFNM       = 0x10, // Special Fully Nested

    PIC_EOI         = 0x20, // End Of Interrupt
};

enum irq_num {
    IRQ_PIT     = 0, // Programmable Interval Timer
    IRQ_KEYB    = 1, // Keyboard
    IRQ_CASCADE = 2, // PIC Cascade - Only used internally
    IRQ_COM2    = 3, // COM 2
    IRQ_COM1    = 4, // COM 1
    IRQ_LPT2    = 5, // LPT 2
    IRQ_FLOP    = 6, // Floppy
    IRQ_LPT1    = 7, // LPT 1
    IRQ_SPUR    = 7, // Spurious interrupt
    IRQ_RTC     = 8, // CMOS Real Time Clock

    IRQ_MOUSE   = 12, // Mouse
    IRQ_COPROC  = 13, // Coprocessor / FPU
    IRQ_ATA_PRI = 14, // Primary ATA
    IRQ_ATA_SEC = 15  // Secondary ATA
};

static inline void pic_send_eoi(u8 irq) {
    if(irq > 7) {
        outb(PIC1_COM, PIC_EOI);
    }
    outb(PIC1_COM, PIC_EOI);
}


/**
 * Initialize the PIC
 */
void pic_init(void);

/**
 * Set interrupt handler for a specified IRQ
 *
 * @param irq IRQ the handler is for
 * @param handler Location of the handler
 */
void set_irq_handler(u8 irq, u32 handler);

/**
 * Disable IRQ
 *
 * @param irq IRQ to disable
 */
void irq_disable(u8 irq);

/**
 * Enable IRQ
 *
 * @param irq IRQ to enable
 */
void irq_enable(u8 irq);

#endif
