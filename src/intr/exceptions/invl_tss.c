#include <io/vid.h>
#include <intr.h>

void hand_invl_tss(u32 eip, u16 seg) __noreturn;

void hand_invl_tss(u32 eip, u16 seg) {
    klog(KLOG_FATAL, "Invalid TSS exception!");
    klog(KLOG_FATAL, "  -> EIP: %08X", eip);
    klog(KLOG_FATAL, "  -> SEG: %04X", seg);
    intr_disable();
    for(;;) intr_hlt();
}
