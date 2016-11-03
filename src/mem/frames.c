#include <mem/frames.h>
#include <io/vid.h>

extern u32 kern_end;     // Located at end of kernel
static u32 mem_avail;    // Amount of memory available

static u32 frames_start; // First available frame
static u32 frames_end;   // Last available frame
static u32 n_frames;     // Number of frames available

static u32 frames_avail[N_FRAME_BITFIELDS] = { 0, }; // Each bit represents one 4-KiB page

void frames_init(u32 mem) {
    mem_avail    = mem;
    frames_start = (u32)(&kern_end + 0x1000) & 0xFFFFF000; // Page-align
    frames_end   = (mem - 1) & 0xFFFFF000; // Page-align
    n_frames     = (frames_end - frames_start) / 4096;
    if(n_frames > N_FRAMES) n_frames = N_FRAMES;
    klog(KLOG_INIT, "frames_start: %08X, frames_end: %08X, n_frames: %04X", frames_start, frames_end, n_frames);
}

void frame_set(u32 frame, u8 used) {
    if(frame > n_frames) {
        klog(KLOG_ERR, "frame_set(%08X, %d): Frame is out of bounds!", frame, used);
        return;
    }
    u32 idx = frame / 32;
    u8  bit = frame & 31;
    if(used) { // Tag frame as used
        frames_avail[idx] |= 1 << bit;
    } else {
        frames_avail[idx] &= !(1 << bit);
    }
}

/**
 * Searches for a consecutive string of 0-bits in a 32-bit bitfield, and returns
 * its index (LSB = 0, MSB = 32) if found, otherwise -1
 * 
 * @param n Bitfield to search through
 * @param size Number of 0-bits to search for
 * @returns Index of string found, otherwise -1
 */
static inline int get_consec_bit_idx(u32 n, u8 size) {
    int i = 0, idx = -1, sz = 0;
    for(; i < 32; i++) {
        if((n & (1 << i)) == 0) {
            if(idx < 0) idx = i;
            sz++;
            if(sz >= size) {
                return idx;
            }
        } else {
            sz = 0;
            idx = -1;
        }
    }
    return -1;
}

u32 frame_find_consec(u8 n) {
    if(n > 32) {
        klog(KLOG_ERR, "frame_find_consec(%d): Do not use this function for allocations greater then 128 KiB!", n);
        return 0;
    }
    u32 i = 0;
    for(; i < N_FRAME_BITFIELDS; i++) {
        if(frames_avail[i] != 0xFFFFFFFF) { // Check if there are any available frames here
            int idx = get_consec_bit_idx(frames_avail[i], n);
            if(idx > -1) {
                return i*32 + (u32)idx;
            }
        }
    }
    klog(KLOG_ERR, "frame_find_consec(%d): Could not find enough free consecutive page frames!", n);
    return 0xFFFFFFFF;
}

/**
 * Get address of frame
 * NOTE: Returns physical address of frame
 */
static inline void *frame_get_addr(u32 frame) {
    return (void *)(frames_start + frame * 4096);
}

/**
 * Get frame number from its address
 * NOTE: Argument is the physical address of the frame
 */
static inline u32 frame_from_addr(void *addr) {
    return (u32)((u32)addr - frames_start) / 4096;
}

void *frame_alloc_consec(u8 n) {
    if(n > 32) {
        klog(KLOG_ERR, "frame_alloc_consec(%d): Do not use this function for allocations greater than 128 KiB!", n);
        return NULL;
    }
    u32 frame = frame_find_consec(n);
    if(frame == 0xFFFFFFFF) {
        klog(KLOG_ERR, "frame_alloc_consec(%d): Could not find enough consecutive free page frames!", n);
        return NULL;
    }

    u32 i = 0;
    for(; i < n; i++) {
        frame_set(frame + i, 1); // Set frames as used
    }
    return frame_get_addr(frame);
}

void frame_free_consec(void *addr, u8 n) {
    u32 frame = frame_from_addr(addr);
    u32 i = 0;
    for(; i < n; i++) {
        frame_set(frame + i, 0);
    }
}
