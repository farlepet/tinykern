#ifndef INTR_EXCEPTIONS_H
#define INTR_EXCEPTIONS_H

void exceptions_init(void); 

enum exceptions {
    EXCEPT_DE  = 0x00, // Divide-by-zero
    EXCEPT_DB  = 0x01, // Debug
    EXCEPT_NMI = 0x02, // Non-maskable Interrupt
    
    EXCEPT_DF  = 0x08, // Double Fault
    EXCEPT_GPF = 0x0D, // General Protection Fault
    EXCEPT_PF  = 0x0E, // Page Fault
};

#endif
