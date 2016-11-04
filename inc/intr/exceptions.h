#ifndef INTR_EXCEPTIONS_H
#define INTR_EXCEPTIONS_H

void exceptions_init(void); 

enum exceptions {
    EXCEPT_DE  = 0x00, // Divide-by-zero
    EXCEPT_DB  = 0x01, // Debug
    EXCEPT_NMI = 0x02, // Non-maskable Interrupt
    EXCEPT_BP  = 0x03, // Breakpoint
    EXCEPT_OF  = 0x04, // Overflow
    EXCEPT_BR  = 0x05, // Bound Range Exceeded
    EXCEPT_UD  = 0x06, // Invalid Opcode
    EXCEPT_NM  = 0x07, // Device Not Available
    EXCEPT_DF  = 0x08, // Double Fault
    EXCEPT_CSO = 0x09, // Coprocessor Segment Overrun NOTE: Not used on the 486 onwards
    EXCEPT_TS  = 0x0A, // Invalid TSS
    EXCEPT_NP  = 0x0B, // Segment Not Present
    EXCEPT_SS  = 0x0C, // Stack-Segment Fault
    EXCEPT_GPF = 0x0D, // General Protection Fault
    EXCEPT_PF  = 0x0E, // Page Fault
    
    EXCEPT_MF  = 0x10, // x86 FPE
    EXCEPT_AC  = 0x11, // Alignment Check
    EXCEPT_MC  = 0x12, // Machine Check
    EXCEPT_XM  = 0x13, // SIMD FPE
    EXCEPT_VE  = 0x14, // Virtualization Exception
    
    EXCEPT_SX  = 0x1E  // Security Exception
};

#endif
