#include <mem/gdt.h>
#include <io/vid.h>
#include <intr.h>

struct gpf_err_code {
    u8 ext:1;   // Exception is external from processor
    u8 ref:2;   // Table referenced (see gpf_ref)
    u16 sel:13; // Selector
} __packed;

enum gpf_ref {
    GPF_REF_GDT  = 0b00, // GDT
    GPF_REF_IDT1 = 0b01, // IDT
    GPF_REF_LDT  = 0b10, // LDT
    GPF_REF_IDT2 = 0b11  // IDT
};

void hand_gpf(u32 eip, struct gpf_err_code err) __noreturn;

void hand_gpf(u32 eip, struct gpf_err_code err) {
    klog(KLOG_ERR, "General Protection Fault occured! EIP: 0x%08X ", eip);

    //struct gpf_err_code code = (struct gpf_err_code)err;
    switch(err.ref) {
        case GPF_REF_GDT:
            if(!err.sel) break;
            klog(KLOG_ERR, "  -> GDT(%d)", err.sel);
            gdt_print_entry(KLOG_ERR, err.sel);
            break;

        case GPF_REF_IDT1:
        case GPF_REF_IDT2:
            klog(KLOG_ERR, "  -> IDT(%d)", err.sel);
            break;

        case GPF_REF_LDT:
            klog(KLOG_ERR, "  -> LDT(%d)", err.sel);
            break;
    }

    klog(KLOG_FATAL, "Halting...");
    intr_disable();
    for(;;) intr_hlt();
}
