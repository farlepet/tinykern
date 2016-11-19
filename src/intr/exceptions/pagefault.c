#include <mem/stack_trace.h>
#include <symbols.h>
#include <io/vid.h>
#include <intr.h>

struct pf_errcode {
    u8 present:1;    // Is page present?
    u8 write:1;      // 1 = write, 0 = read
    u8 user:1;       // Did CPL==3 ?
    u8 resv_w:1;     // Read a 1 in a reserved field
    u8 ifetch:1;     // Instruction Fetch
    u32 reserved:27; // Reserved
} __packed;

void hand_pagefault(u32 eip, struct pf_errcode err, u32 cr2, u32 *ebp) __noreturn;

void hand_pagefault(u32 eip, struct pf_errcode err, u32 cr2, u32 *ebp) {
    klog(KLOG_ERR, "Page Fault at 0x%08X", cr2);
    symbol_t *obj = sym_find_object(cr2);
    if(obj != NULL) {
        klog(KLOG_ERR, "  -> Object: %s", obj->name);
    }
    klog(KLOG_ERR, "  -> Stack addr: %08X", ebp);
    klog(KLOG_ERR, "  -> Caused by instruction at 0x%08X", eip);
    klog(KLOG_ERR, "  -> %sPresent", err.present ? "" : "Not ");
    klog(KLOG_ERR, "  -> %s", err.write ? "Write" : "Read");
    klog(KLOG_ERR, "  -> %s", err.user ? "User" : "System");
    if(err.resv_w) klog(KLOG_ERR, "  -> Reserved field set to 1");
    if(err.ifetch) klog(KLOG_ERR, "  -> Instruction Fetch");

    if(ebp != NULL) {
        stack_trace(5, ebp, eip);
    }
    
    klog(KLOG_FATAL, "Halting...");
    intr_disable();
    for(;;) intr_hlt();
}
