#include <mem/gdt.h>
#include <io/vid.h>

static gdt_entry_t GDT[3];

void gdt_init() {
    klog(KLOG_INIT, "Creating GDT");
    gdt_create_entry(&GDT[1], 0, 0xFFFFF, GDT_ACC_RW | GDT_ACC_EXEC | GDT_ACC_PRESENT, GDT_FLAG_SIZE_32B | GDT_FLAG_GRAN_4K);
    gdt_create_entry(&GDT[2], 0, 0xFFFFF, GDT_ACC_RW | GDT_ACC_PRESENT, GDT_FLAG_SIZE_32B | GDT_FLAG_GRAN_4K);
    gdt_desc_t gdesc = { .size = (sizeof(GDT) - 1), .offset = (u32)GDT };
    klog(KLOG_INITINFO, "Loading GDT");
    gdt_lgdt(&gdesc);
    klog(KLOG_INITINFO, "Reloading segments");
    gdt_seg_reload();
    klog(KLOG_INIT, "GDT successfully created");
}

void gdt_create_entry(gdt_entry_t *ent, u32 base, u32 limit, u8 access, u8 flags) {
    klog(KLOG_INITINFO, "Creating GDT entry");

    ent->base_low  = (u16)base;
    ent->base_med  = (u8)(base >> 16);
    ent->base_high = (u8)(base >> 24);

    ent->limit_low  = (u16)limit;
    ent->limit_high = (u8)(limit >> 16);

    ent->access = access | GDT_ACC_ONE;
    ent->flags  = flags;
}
