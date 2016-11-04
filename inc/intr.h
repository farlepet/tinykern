#ifndef INTR_H
#define INTR_H

#include <types.h>

#include <intr/exceptions.h>
#include <intr/pic.h>
#include <intr/idt.h>

/**
 * Enable Interrupts
 */
static inline void intr_enable() {
    asm volatile("sti");
}

/**
 * Disable interrupts
 */
static inline void intr_disable() {
    asm volatile("cli");
}

/**
 * Halt until next interrupt
 */
static inline void intr_hlt() {
    asm volatile("hlt");
}

#define __INT(N) asm volatile("int $"#N)

#endif
