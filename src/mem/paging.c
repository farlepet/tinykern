#include <mem/paging.h>
#include <mem/frames.h>
#include <io/vid.h>
#include <mem.h>

static pdir_ent_t pdir[1024] __aligned(4096);
static ptable_ent_t ptable_4M[1024] __aligned(4096);

static void paging_clear_dir(pdir_ent_t *dir);
static void paging_fill_table(ptable_ent_t *table, u32 phys);

void paging_init() {
    klog(KLOG_INIT, "Setting up paging");
    paging_clear_dir(pdir);
    
    paging_fill_table(ptable_4M, 0x00000000);
    pdir[0].i = (u32)ptable_4M;
    pdir[0].b.present = 1;
    pdir[0].b.rw = 1;
    
    paging_set_dir(pdir);
    paging_enable();
    klog(KLOG_INIT, "Paging enabled");
}

static void paging_clear_dir(pdir_ent_t *dir) {
    u32 i = 0;
    for(; i < 1024; i++) {
        dir[i].i = 0x00000002;
    }
}

static void paging_fill_table(ptable_ent_t *table, u32 phys) {
    u32 i = 0;
    for(; i < 1024; i++) { 
        table[i].i = (phys + i * 4096) | 0x03; // Present, RW
    }
}

u32 paging_get_phys_addr(u32 virtaddr) {
    u32 offset = virtaddr & 0xFFF;
    u32 dir_idx = virtaddr >> 22;
    u32 tab_idx = (virtaddr >> 12) & 0x03FF;
    ptable_ent_t *table = (ptable_ent_t *)(pdir[dir_idx].i & 0xFFFFF000);
    return (table[tab_idx].i & 0xFFFFF000) | offset;
}

void paging_map_page(void *phys, void *virt, u32 flags) {
    phys = (void *)((u32)phys & 0xFFFFF000);
    virt = (void *)((u32)virt & 0xFFFFF000);
    
    u32 dir_idx = (u32)virt >> 22;
    u32 tab_idx = ((u32)virt >> 12) & 0x03FF;

    if(pdir[dir_idx].b.present) { // Table exists
        ((u32 *)(pdir[dir_idx].i & 0xFFFFF000))[tab_idx] = ((u32)phys | (flags & 0xFFF) | 0x01);
    } else { // Table doesn't exist: we must create it
        // Do we need to map this inside a page as well?
        void *table = frame_alloc_consec(1); // Allocate a page
        pdir[dir_idx].i = (u32)table | 0x03;
        
        memset(table, 0, 4096);

        ((u32 *)(pdir[dir_idx].i & 0xFFFFF000))[tab_idx] = ((u32)phys | (flags & 0xFFF) | 0x01);
    }
}
