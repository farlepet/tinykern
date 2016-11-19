#include <mem/paging.h>
#include <mem/alloc.h>
#include <io/vid.h>
#include <mem.h>

extern u32 kern_end;
static u32 mem_avail;

static u32 mem_start;
static u32 mem_end;

static memblk_t mem_block1[MEMBLK_ENTRIES];
static memblk_t *mem_blocks[MEMBLK_NBLOCKS] = { mem_block1, 0, };
static u32 n_memblk_blocks = 1;

void alloc_init(u32 mem) {
    mem_avail = mem;
    mem_start = (u32)(&kern_end + 0x1000) & 0xFFFFF000;
    mem_end   = mem - 1;

    mem_blocks[0][0].addr  = mem_start;
    mem_blocks[0][0].size  = mem_end - mem_start;
    mem_blocks[0][0].used  = 0;
    mem_blocks[0][0].valid = 1;
}

/**
 * Get the number of empty (invalid) entries
 */
static u32 empty_entries() {
    u32 i = 0, j, ent = 0;
    for(; i < n_memblk_blocks; i++) {
        for(j = 0; j < MEMBLK_ENTRIES; j++) {
            if(mem_blocks[i][j].valid == 0) {
                ent++;
            }
        }
    }
    return ent;
}

/**
 * Allocate a block of memory entries
 */
static void alloc_mem_block() {
    if(n_memblk_blocks == MEMBLK_NBLOCKS) {
        klog(KLOG_ERR, "alloc_mem_block: Maximum number of memblk blocks (%d) exceeded!", MEMBLK_NBLOCKS);
        return; // TODO: Actual error handling
    }
    void *frame = kalloc(4096);
    memset(frame, 0, 4096);
    mem_blocks[n_memblk_blocks++] = (memblk_t *)frame;
}

/**
 * Find a memory entry with an appropiate size
 *
 * TODO: Look for smallest entry, instead of the first
 *
 * @param blk Pointer to variable in which to store the block
 * @param ent Pointer to variable in which to store the entry
 * @param length Length of memory region to find
 * @param alignment Alignment of the memory region
 */
static int find_free_entry(u32 *blk, u32 *ent, u32 length, u32 alignment) {
    u32 i = 0, j;
    for(; i < n_memblk_blocks; i++) {
        for(j = 0; j < MEMBLK_ENTRIES; j++) {
            if(mem_blocks[i][j].valid && !mem_blocks[i][j].used) {
                u32 len = length;
                if(alignment > 1) {
                    len += (((mem_blocks[i][j].addr + alignment) / alignment) * alignment - mem_blocks[i][j].addr);
                }
                if(mem_blocks[i][j].size > len) {
                    *blk = i;
                    *ent = j;
                    return 1;
                }
            }
        }
    }
    return 0; // No free blocks found
}

/**
 * Split memory region into two entries
 *
 * @param blk Memory entry block
 * @param ent Memory entry
 * @param length Length of lower half (see direction)
 * @param direction 0=move upper half, 1=move lower half
 */
static void split_mem_entry(u32 blk, u32 ent, u32 length, u32 direction) {
    memblk_t *orig_blk = &(mem_blocks[blk][ent]);
    if(orig_blk->size == length) {
        return; // Nothing to do, block is already the correct size
    }
    u32 nsize = orig_blk->size - length;
    u32 i = 0, j;
    for(; i < n_memblk_blocks; i++) {
        for(j = 0; j < MEMBLK_ENTRIES; j++) {
            if(mem_blocks[i][j].valid == 0) {
                if(direction == 0) {
                    mem_blocks[i][j].addr = orig_blk->addr + length;
                    mem_blocks[i][j].size = nsize;
                    orig_blk->size        = length;
                } else {
                    mem_blocks[i][j].addr = orig_blk->addr;
                    mem_blocks[i][j].size = length;
                    orig_blk->addr        = orig_blk->addr + length;
                    orig_blk->size        = nsize;
                }
                mem_blocks[i][j].valid = 1;
                mem_blocks[i][j].used  = 0;
                return;
            }
        }
    }
    // We need to allocate another block
    alloc_mem_block();

    mem_blocks[n_memblk_blocks][0].addr  = orig_blk->addr + length;
    mem_blocks[n_memblk_blocks][0].size  = nsize;
    mem_blocks[n_memblk_blocks][0].valid = 1;
    mem_blocks[n_memblk_blocks][0].used  = 0;
}

void *kalloc(u32 len) {
    if(empty_entries() < 4) alloc_mem_block(); // Make sure we always have free blocks
    u32 blk, ent;
    if(!find_free_entry(&blk, &ent, len, 1)) {
        klog(KLOG_ERR, "kalloc: Could not allocate %d bytes of memory!", len);
        return NULL;
    }
    split_mem_entry(blk, ent, len, 0);
    mem_blocks[blk][ent].used = 1;

    u32 i = 0;
    for(; i < len; i+= 0x1000) {
        paging_map_page((void *)(mem_blocks[blk][ent].addr + i), (void *)(mem_blocks[blk][ent].addr + i), 0x03);
    }

    return (void *)mem_blocks[blk][ent].addr;
}

void *kalloc_frame(u32 len) {
    if(empty_entries() < 4) alloc_mem_block(); // Make sure we always have free blocks
    u32 blk, ent;
    if(!find_free_entry(&blk, &ent, len, 0x1000)) {
        klog(KLOG_ERR, "kalloc_frame: Could not allocate %d bytes of page-aligned memory!", len);
        return NULL;
    }
    memblk_t *entry = &mem_blocks[blk][ent];

    u32 off = ((entry->addr + 0x1000) / 0x1000) * 0x1000 - entry->addr;
    split_mem_entry(blk, ent, off, 1); // Remove lower part
    split_mem_entry(blk, ent, len, 0); // Remove upper part
    entry->used = 1;
    return (void *)entry->addr;
}
