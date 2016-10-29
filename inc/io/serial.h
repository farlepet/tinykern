#ifndef SERIAL_H
#define SERIAL_H

#include <types.h>

// NOTE: The following may not be true for all machines!
enum serial_port {
    SER_NULL = 0x000,
    SER_COM1 = 0x3F8,
    SER_COM2 = 0x2F8,
    SER_COM3 = 0x3E8,
    SER_COM4 = 0x2E8
};


/**
 * Initialize a serial port
 *
 * @param port The base IO address of the serial port to initialize
 * @param baud The baud rate for the serial port to use
 */
void serial_init(enum serial_port port, u16 baud);

/**
 * Read a character from the serial port
 * NOTE: This is currently a blocking function, and will stall execution 
 * until a character is recieved.
 *
 * @param port The base IO address of the serial port to read from
 * @return The value received by the serial port
 */
char serial_getc(enum serial_port port);

/**
 * Write a character to the serial port
 * 
 * @param port The base IO address of the serial port to write to
 * @param ch The character to write to the serial port
 */
void serial_putc(enum serial_port port, char ch);

/**
 * Write bytes from a buffer to the serial port
 *
 * @param port The base IO address of the serial port to write to
 * @param buf Buffer containing bytes to write
 * @param len Number of bytes to write
 */
void serial_write(enum serial_port port, const u8 *buf, int len);


// Get the 16-bit divisor for any given baud rate
#define serial_get_divisor(B) (u16)(115200 / B)

enum serial_regs {
    SER_REG_DATA = 0, // Data transmit/recieve (THR/RBR)
    SER_REG_IER  = 1, // Interrupt enable (IER)
    SER_REG_IIR  = 2, // Interrupt ID (read-only) (IIR)
    SER_REG_FCR  = 2, // FIFO register (write-only) (FCR)
    SER_REG_LCR  = 3, // Line control (LCR)
    SER_REG_MCR  = 4, // Modem control (MCR)
    SER_REG_LSR  = 5, // Line status (LSR)
    SER_REG_MSR  = 6, // Modem status (MSR)
    SER_REG_SR   = 7, // Scratch register (SR)

    // When DLAB = 0
    SER_REG_DLL  = 0, // LSB of divisor (DLL)
    SER_REG_DLH  = 1  // MSB of divisor (DLH)
};

// Interrupt enable register (IER) bits
enum serial_ier_bits {
    SER_IER_DATA_AVAIL  = 0b00000001, // Data available interrupt
    SER_IER_TRANS_EMPTY = 0b00000010, // Transmitter holding register empty interrupt
    SER_IER_LINE_STAT   = 0b00000100, // Reciever line status interrupt
    SER_IER_MODM_STAT   = 0b00001000, // Modem status interrupt
    SER_IER_EN_SLEEPM   = 0b00010000, // Enable sleep mode
    SER_IER_EN_LOWPWR   = 0b00100000  // Enable low-power mode
};

// Interrupt ID (IIR) bits
enum serial_iir_bits {
    SER_IIR_INTR_PEND = 0b00000001, // Interrupt pending
    SER_IIR_INTR_TYPE = 0b00001110, // Interrupt type
    SER_IIR_64B_FIFO  = 0b00100000, // 64-byte FIFO enabled
    SER_IIR_FIFO      = 0b11000000, // FIFO information

    SER_INTR_TYPE_MODM_STAT   = 0b00000000, // Modem status
    SER_INTR_TYPE_TRANS_EMPTY = 0b00000010, // Transmitter holding register empty
    SER_INTR_TYPE_DATA_AVAIL  = 0b00000100, // Recieved data availabe
    SER_INTR_TYPE_LINE_STAT   = 0b00000110, // Line status
    SER_INTR_TYPE_TIMEOUT     = 0b00001100  // Timeout interrupt pending
};

// FIFO control (FCR) bits
enum serial_fcr_bits {
    SER_FCR_FIFO_ENABLE = 0b00000001, // FIFO Enable
    SER_FCR_CLR_RECV    = 0b00000010, // Clear Receive FIFO
    SER_FCR_CLR_TRANS   = 0b00000100, // Clear Transmit FIFO
    SER_FCR_DMA_MODE    = 0b00001000, // DMA Mode Select
    SER_FCR_64BYTE_EN   = 0b00100000, // 64-Byte FIFO Enable
    SER_FCR_TRIG_LVL    = 0b11000000, // Interrupt Trigger Level (See SER_FCR_TRIG_*)

    SER_FCR_TRIG_1B     = 0b00000000,
    // 16-Byte FIFO:
    SER_FCR_TRIG_4B     = 0b01000000,
    SER_FCR_TRIG_8B     = 0b10000000,
    SER_FCR_TRIG_14B    = 0b11000000,
    // 64-Byte FIFO:
    SER_FCR_TRIG_16B    = 0b01000000,
    SER_FCR_TRIG_32B    = 0b10000000,
    SER_FCR_TRIG_56B    = 0b11000000
};

// Line control register (LCR) bits:
enum serial_lcr_bits {
    SER_LCR_WORD_LEN  = 0b00000011, // Word length
    SER_LCR_STOP_BITS = 0b00000100, // Stop bits
    SER_LCR_PARITY    = 0b00111000, // Parity
    SER_LCR_BREAK_EN  = 0b01000000, // Break enable
    SER_LCR_DLAB      = 0b10000000, // Divisor Latch Access Bit

    // Word lengths:
    SER_LCR_WL_5B     = 0b00000000,
    SER_LCR_WL_6B     = 0b00000001,
    SER_LCR_WL_7B     = 0b00000010,
    SER_LCR_WL_8B     = 0b00000011
};

// Modem control register (MCR) bits:
enum serial_mcr_bits {
    SER_MCR_DTR  = 0b00000001, // Data Terminal Ready
    SER_MCR_RTS  = 0b00000010, // Request To Send
    SER_MCR_AUX1 = 0b00000100, // Auxilliary Output 1
    SER_MCR_AUX2 = 0b00001000, // Auxilliary Output 2
    SER_MCR_LOOP = 0b00010000, // Loopback Mode
    SER_MCR_AFC  = 0b00100000  // Autoflow control enable
};

// Line Status Register (LSR) bits:
enum serial_lsr_bits {
    SER_LSR_DATA_READY = 0b00000001, // Data Ready
    SER_LSR_OVRRUN_ERR = 0b00000010, // Overrun Error
    SER_LSR_PARITY_ERR = 0b00000100, // Parity Error
    SER_LSR_FRAME_ERR  = 0b00001000, // Framing Error
    SER_LSR_BREAK_INTR = 0b00010000, // Break Interrupt
    SER_LSR_EMPT_TRANS = 0b00100000, // Empty Transmitter Holding Register
    SER_LSR_EMPT_DATA  = 0b01000000, // Empty Data Holding Registers
    SER_LSR_RFIFO_ERR  = 0b10000000  // Error in revieved FIFO
};

// Modem Status Register (MSR) bits:
enum serial_msr_bits {
    SER_MSR_DELTA_CTS = 0b00000001, // Delta Clear To Send
    SER_MSR_DELTA_DSR = 0b00000010, // Delta Data Set Ready
    SER_MSR_DELTA_RI  = 0b00000100, // Delta Ring Indicator (1 to 0 only)
    SER_MSR_DELTA_CD  = 0b00001000, // Delta Carrier Detect
    SER_MSR_CTS       = 0b00010000, // Clear To Send
    SER_MSR_DSR       = 0b00100000, // Data Set Ready
    SER_MSR_RI        = 0b01000000, // Ring Indicator
    SER_MSR_CD        = 0b10000000  // Carrier Detect
};




#endif
