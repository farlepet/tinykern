#ifndef INTR_IDT_H
#define INTR_IDT_H

#include <types.h>

typedef struct idt_desc {
    u16 size;   // Size of IDT - 1
    u32 offset; // Location of IDT
} __packed idt_desc_t;

extern void idt_lidt(idt_desc_t *desc);

typedef struct idt_entry {
    u16 off_low;  // Lower 16-bits of offset
    u16 sel;      // Selector
    u8 zero;      // Always zero
    u8 flags;     // Flags
    u16 off_high; // Upper 16-bits of offset
} __packed idt_entry_t;

extern idt_entry_t IDT[256];

enum idt_flag_bits {
    IDT_FLAG_GATE_TYPE = 0b00001111,  // Gate type
    IDT_FLAG_STOR_SEG  = 0b00010000,  // Storage segment (0 for interrupts)
    IDT_FLAG_PRIV      = 0b01100000,  // Required priveledge level
    IDT_FLAG_PRESENT   = 0b10000000,  // Present

    IDT_FLAG_TYPE_TASK32 = 0b0101,    // 32-bit Task Gate
    IDT_FLAG_TYPE_INTR16 = 0b0110,    // 16-bit Interrupt Gate
    IDT_FLAG_TYPE_TRAP16 = 0b0111,    // 16-bit Trap Gate
    IDT_FLAG_TYPE_INTR32 = 0b1110,    // 32-bit Interrupt Gate
    IDT_FLAG_TYPE_TRAP32 = 0b1111,    // 16-bit Trap Gate

    IDT_FLAG_PRIV_RING0 = 0b00000000, // Callable by ring 0 only
    IDT_FLAG_PRIV_RING1 = 0b00100000, // Callable by ring 1 and below
    IDT_FLAG_PRIV_RING2 = 0b01000000, // Callable by ring 2 and below
    IDT_FLAG_PRIV_RING3 = 0b01100000  // Callable anywhere
};

void idt_init(void);

void idt_create_entry(idt_entry_t *ent, u32 offset, u16 selector, u8 flags);

#endif
