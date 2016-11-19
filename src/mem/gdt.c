#include <mem/gdt.h>
#include <io/vid.h>
#include <mem.h>

static gdt_entry_t GDT[4];
static u8 system_stack[0x2000]; // Stack for interrupts
static tss_struct_t TSS1; // TSS

void gdt_init() {
    klog(KLOG_INIT, "Creating GDT");
    gdt_create_entry(&GDT[1], 0, 0xFFFFF, GDT_ACC_RW | GDT_ACC_EXEC | GDT_ACC_ONE | GDT_ACC_PRESENT, GDT_FLAG_SIZE_32B | GDT_FLAG_GRAN_4K);
    gdt_create_entry(&GDT[2], 0, 0xFFFFF, GDT_ACC_RW | GDT_ACC_ONE | GDT_ACC_PRESENT, GDT_FLAG_SIZE_32B | GDT_FLAG_GRAN_4K);
    gdt_create_entry(&GDT[3], (u32)&TSS1, sizeof(tss_struct_t), 0x89, GDT_FLAG_SIZE_32B);

    TSS1.esp0 = (u32)system_stack + sizeof(system_stack);
    TSS1.ss0  = 0x10;
    TSS1.cs   = 0x08;
    TSS1.es   = TSS1.ss = 0x10;
    TSS1.gs   = TSS1.fs = 0x10;
    TSS1.ds   = 0x10;
    TSS1.iomap = sizeof(tss_struct_t);
    
    gdt_desc_t gdesc = { .size = (sizeof(GDT) - 1), .offset = (u32)GDT };
    klog(KLOG_INITINFO, "Loading GDT");
    gdt_lgdt(&gdesc);
    klog(KLOG_INITINFO, "Reloading segments");
    gdt_seg_reload();

    klog(KLOG_INITINFO, "Loading TSS");
    gdt_load_tss();
    klog(KLOG_INIT, "GDT successfully created");
}

void gdt_create_entry(gdt_entry_t *ent, u32 base, u32 limit, u8 access, u8 flags) {
    klog(KLOG_INITINFO, "Creating GDT entry");

    ent->base_low  = (u16)base;
    ent->base_med  = (u8)(base >> 16);
    ent->base_high = (u8)(base >> 24);

    ent->limit_low  = (u16)limit;
    ent->limit_high = (u8)(limit >> 16);

    ent->access = access;
    ent->flags  = flags;
}

void gdt_print_entry(enum klog_level elvl, u16 idx) {
    u32 base  = GDT[idx].base_low | (u32)(GDT[idx].base_med << 16) | (u32)(GDT[idx].base_high << 24);
    u32 limit = GDT[idx].limit_low | (u32)(GDT[idx].limit_high << 16);
    klog(elvl, "GDT(%d): BASE: 0x%08X", idx, base);
    klog(elvl, "        LIMIT: 0x  %06X", limit);
    klog(elvl, "        FLAGS: 0x%02X ACCESS: %02X", GDT[idx].flags, GDT[idx].access);
}

