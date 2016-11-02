#ifndef MEM_GDT_H
#define MEM_GDT_H

#include <proc/tss.h>
#include <io/vid.h>
#include <types.h>

typedef struct gdt_desc {
    u16 size;   // Size of GDT - 1
    u32 offset; // Location of GDT
} __packed gdt_desc_t;

typedef struct gdt_entry {
    u16 limit_low:16; // Lower 16 bits of limit
    u16 base_low:16;  // Lower 16 bits of base
    u8  base_med:8;   // Middle 8 bits of base
    u8  access:8;     // Access bits
    u8  limit_high:4; // Upper 4 bits of limit
    u8  flags:4;      // Flag bits
    u8  base_high:8;  // Upper 8 bits of base

} __packed gdt_entry_t;

enum gdt_flag_bits {
    GDT_FLAG_SIZE     = 0b0100, // Size bit
    GDT_FLAG_GRAN     = 0b1000, // Granularity bit

    GDT_FLAG_SIZE_16B = 0b0000, // 16-bit protected mode
    GDT_FLAG_SIZE_32B = 0b0100, // 32-bit protected mode

    GDT_FLAG_GRAN_1B  = 0b0000, // 1-byte granularity
    GDT_FLAG_GRAN_4K  = 0b1000  // 4K granularity

};


enum gdt_access_bits {
    GDT_ACC_ACCESSED = 0b00000001, // Whether or not the segment was accessed (Do not set this bit)
    GDT_ACC_RW       = 0b00000010, // Data: Write Enable, Code: Read Enable
    GDT_ACC_DC       = 0b00000100, // Data: Direction (1 = segment grows downward from offset) Code: Conformity (1 = lower priviledge levels can execute code in segment)
    GDT_ACC_EXEC     = 0b00001000, // Whetehr not data in segment is executable
    GDT_ACC_ONE      = 0b00010000, // Always set this to one
    GDT_ACC_PRIVL    = 0b01100000, // Priviledge level
    GDT_ACC_PRESENT  = 0b10000000, // Present (set to 1)

    GDT_ACC_RING0    = 0b00000000, // Ring 0 (kernel)
    GDT_ACC_RING1    = 0b00100000, // Ring 1
    GDT_ACC_RING2    = 0b01000000, // Ring 2
    GDT_ACC_RING3    = 0b01100000  // Ring 3 (lowest priviledges)
};




/**
 * Load the Global Descriptor Table
 *
 * @param desc Pointer to the GDT
 */
extern void gdt_lgdt(gdt_desc_t *desc);

/**
 * Update and reload segments to match the new GDT
 */
extern void gdt_seg_reload(void);

/**
 * Tell the processor where the TSS is located withing the GDT
 */
extern void gdt_load_tss(void);


/**
 * Initialize and load the Global Descriptor Table
 */
void gdt_init(void);

/**
 * Fills in a GDT entry pointed to by `ent`
 *
 * @param ent Pointer to GDT entry
 * @param base Base address of the entry
 * @param limit Upper limit of entry
 * @param access Access bits
 * @param flags Flags
 */
void gdt_create_entry(gdt_entry_t *ent, u32 base, u32 limit, u8 access, u8 flags);



void gdt_print_entry(enum klog_level elvl, u16 idx);

#endif
