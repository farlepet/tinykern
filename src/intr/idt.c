#include <intr/idt.h>
#include <io/vid.h>
#include <intr.h>

idt_entry_t IDT[256];

extern void intr_test_intr(void);
extern void intr_dummy_int(void);

void idt_init() {
    klog(KLOG_INIT, "Setting up IDT");
    int i = 0;
    for(; i < 256; i++) {
        idt_create_entry(&IDT[i], 0, 0, 0);
    }
    idt_create_entry(&IDT[0], (u32)intr_test_intr, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_TYPE_INTR32); 
    idt_desc_t desc = { .size = (sizeof(IDT) - 1), .offset = (u32)IDT };
    idt_lidt(&desc);
    klog(KLOG_INIT, "IDT loaded");
}

void idt_create_entry(idt_entry_t *ent, u32 offset, u16 selector, u8 flags) {
    ent->off_low  = (u16)offset;
    ent->off_high = (u16)(offset >> 16);

    ent->zero = 0;
    ent->sel = selector;
    ent->flags = flags;
}
