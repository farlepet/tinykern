#ifndef PROC_TSS_H
#define PROC_TSS_H

#include <types.h>

typedef struct tss_struct {
    u16 link;
    u16 link_h;

    u32 esp0;
    u16 ss0;
    u16 ss0_h;
    
    u32 esp1;
    u16 ss1;
    u16 ss1_h;
    
    u32 esp2;
    u16 ss2;
    u16 ss2_h;

    u32 cr3;
    u32 eip;
    u32 eflags;

    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;

    u32 esp;
    u32 ebp;

    u32 esi;
    u32 edi;

    u16 es;
    u16 es_h;

    u16 cs;
    u16 cs_h;

    u16 ss;
    u16 ss_h;

    u16 ds;
    u16 ds_h;

    u16 fs;
    u16 fs_h;

    u16 gs;
    u16 gs_h;

    u16 ldt;
    u16 ldt_h;

    u16 trap;
    u16 iomap;
} __packed tss_struct_t;


#endif
