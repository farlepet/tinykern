#include <mem/stack_trace.h>
#include <mem/paging.h>
#include <symbols.h>
#include <io/vid.h>

static void stack_trace_print_data(u32 *ebp, u32 len);
static int stack_trace_print_func(u32 addr);
static int find_function(u32 addr);
extern void _start(void);

void stack_trace(u32 max_frames, u32 *ebp, u32 saved_eip) {
    if(ebp == NULL) {
        return;
    }
    u32 *oebp; // old ebp

    kprintf("Stack Trace:\n");
    if(saved_eip) stack_trace_print_func(saved_eip);
    u32 fr = 0;
    for(; fr < max_frames; fr++) {
        if(!page_present((u32)ebp)) {
            kprintf("  EBP[%08x] points to non-present page!\n");
            break;
        }
        u32 eip = ebp[1];
        if(stack_trace_print_func(eip) < 0) break;
        oebp = ebp;
        ebp = (u32 *)ebp[0];
        u32 frame_size = (u32)(ebp - oebp);
        stack_trace_print_data(ebp, frame_size);
    }
}

static int stack_trace_print_func(u32 eip) {
    if(eip == 0) return -1;
    int idx = find_function(eip);
    if(idx >= 0) {
        u32 addr = sym_functions[idx].addr + (u32)&kern_start;
        kprintf("  [%08x] <0x%08x +%3d> %s\n", eip, addr, eip - addr, sym_functions[idx].name);
        if(addr == (u32)&_start) {
            kprintf("  -- End of kernel stack --\n");
            return -1;
        }
    } else {
        kprintf("  [%08x] %s\n", eip, page_present(eip) ? "" : "[np]");
    }
    return 0;
}

static void stack_trace_print_data(u32 *ebp, u32 len) {
    len >>= 2;
    kprintf("  -> ");
    u32 i = 0;
    for(; i < len; i++) {
        if(i != 0 && (i % 4) == 0) {
            kprintf("\n     ");
        }
        kprintf("%08X ", ebp[-(i+1)]);
    }
    kprintf("\n");
}

static int find_function(u32 addr) {
    addr -= (u32)&kern_start;
    int i = 0;
    int idx = -1;
    u32 laddr = 0;
    symbol_t *func = NULL;
    while((func = &sym_functions[i])->addr != 0xFFFFFFFF) {
        if(func->addr <= addr) {
            if(func->addr > laddr) {
                if(func->size > 0) {
                    if(func->addr + func->size > addr) {
                        laddr = func->addr;
                        idx = i;
                    }
                } else {
                    laddr = func->addr;
                    idx = i;
                }
            }
        }
        i++;
    }
    return idx;
}

