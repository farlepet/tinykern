#ifndef PAGING_H
#define PAGING_H

#include <types.h>

struct pdir_ent {
    u8  present:1;       // Present
    u8  rw:1;            // Writable
    u8  user:1;          // User-accessible
    u8  writethrough:1;  // Write-through caching
    u8  cache_disable:1; // Disable caching
    u8  accessed:1;      // Accessed
    u8  zero1:1;         // Always zero
    u8  size:1;          // Size of page (0 = 4K, 1 = 4M)
    u8  zero2:1;         // Always zero
    u8  unused:3;        // Unused by CPU, free for OS use
    u32 address:20;      // 4K-aligned address of page-table
} __packed;

typedef union {
    struct pdir_ent b;   // Bitfield access
    u32 i;               // Integer access
} pdir_ent_t;

struct ptable_ent {
    u8 present:1;       // Present
    u8 rw:1;            // Writable
    u8 user:1;          // User-accessible
    u8 writethrough:1;  // Write-through cachine
    u8 cache_disable:1; // Disable caching
    u8 accessed:1;      // Accessed
    u8 dirty:1;         // Page has been written to
    u8 zero:1;          // Always zero
    u8 global:1;        // Global page
    u8 unused:3;        // Unused by CPU, free for OS use
    u32 address:20;     // 4K-aligned address of page
} __packed;

typedef union {
    struct ptable_ent b; // Bitfield access
    u32 i;               // Integer access
} ptable_ent_t;


/**
 * Set up paging
 */
void paging_init(void);

/**
 * Get the physical address from a virtual address
 *
 * @param virtaddr Virtual address
 * @return Physical address
 */
u32 paging_get_phys_addr(u32 virtaddr);


/**
 * Enable paging
 */
extern void paging_enable(void);

/**
 * Disable paging
 */
extern void paging_disable(void);

/**
 * Set page directory (CR3)
 *
 * @param dir Pointer to page directory
 */
extern void paging_set_dir(pdir_ent_t *dir);

/**
 * Map a virtual address to a physical address via paging
 *
 * @param phys Physical address
 * @param virt Virtual address
 * @param flags Page flags
 */
void paging_map_page(void *phys, void *virt, u32 flags);

#endif
