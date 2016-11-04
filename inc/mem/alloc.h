#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <types.h>

typedef struct memblk {
    u8  valid; // Is this memory block valid?
    u8  used;  // Is this memory block in use?
    u16 pad;   // Padding
    u32 addr;  // Memory block physical address
    u32 size;  // Memory block size
} memblk_t;

#define MEMBLK_ENTRIES (4096 / sizeof(memblk_t)) // So it fits nicely in one page
#define MEMBLK_NBLOCKS (1024)                    // Maximum number of blocks of entries

/**
 * Initialize memory allocation system
 *
 * TODO: Get more specific memory info from multiboot, and support
 * non-consecutive blocks of memory.
 *
 * @param mem Amount of consecutive memory available to the system
 */
void alloc_init(u32 mem);

/**
 * Allocate a block of memory
 *
 * @param len Size of block in bytes
 */
void *kalloc(u32 len);

/**
 * Allocate a block of memory starting on a page boundary
 *
 * @param len Size of block in bytes
 */
void *kalloc_frame(u32 len);

#endif
