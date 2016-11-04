#ifndef INTR_IRQ_PIT_H
#define INTR_IRQ_PIT_H

#include <types.h>

/**
 * Initialize Programmable Interrupt Timer and install IRQ handler
 */
void pit_init(u32 freq);

#endif
