#include <io/serial.h>
#include <io/ioport.h>

void serial_init(enum serial_port port, u16 baud) {
    u16 div = serial_get_divisor(baud);
    outb(port + SER_REG_IER, 0x00); // Disable interrupts
    outb(port + SER_REG_LCR, SER_LCR_DLAB); // Set DLAB bit
    // Set divisor
    outb(port + SER_REG_DLL, (u8)div);
    outb(port + SER_REG_DLH, (u8)(div >> 8));
    outb(port + SER_REG_LCR, SER_LCR_WL_8B); 
    outb(port + SER_REG_FCR, SER_FCR_FIFO_ENABLE | SER_FCR_CLR_RECV | SER_FCR_CLR_TRANS | SER_FCR_TRIG_14B); // Enable FIFO
}

// Check if data is available
static inline int serial_data_ready(enum serial_port port) {
    return inb(port + SER_REG_LSR) & SER_LSR_DATA_READY;
}

char serial_getc(enum serial_port port) {
    while(!serial_data_ready(port)) {}
    return (char)inb(port);
}

// Check if serial port is ready to transmit
static inline int serial_empt_trans(enum serial_port port) {
    return inb(port + SER_REG_LSR) & SER_LSR_EMPT_TRANS;
}

void serial_putc(enum serial_port port, char ch) {
    while(!serial_empt_trans(port)) {}
    outb(port, (u8)ch);
}
