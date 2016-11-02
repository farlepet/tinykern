#include <io/vid.h>
#include <intr.h>

void hand_doublefault(void) __noreturn;

void hand_doublefault() {
    kprintf("Encountered a double-fault, halting!");
    intr_disable();
    while(1) intr_hlt();
}
