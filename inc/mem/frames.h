/**
 * Physical address frame allocation functions
 */

#ifndef MEM_FRAMES_H
#define MEM_FRAMES_H

#include <types.h>

#define N_FRAME_BITFIELDS 4096 // Able to represent 512 MiB of memory
#define N_FRAMES (N_FRAME_BITFIELDS * 32)

/**
 * Initialize frame allocation system
 *
 * @param mem Amount of (consecutive) memory available for use
 */
void frames_init(u32 mem);

/**
 * Mark a frame as either used or unused
 *
 * @param frame Frame to mark
 * @param used Whether to mark frame as used (1) or unused (0)
 */
void frame_set(u32 frame, u8 used);

/**
 * Find a consecutive string of available fames, and return the frame
 *
 * TODO: Support lengths larger than 4KiB
 * TODO: Support bit strings across 32-bit boundries
 *
 * @param n_frames Number of consecutive frames to find
 * @return First frame in string of free frames, else 0xFFFFFFFF
 */
u32 frame_find_consec(u8 n_frames);

/**
 * Allocate a consecutive string of page frames, and return corresponding address
 *
 * @param n_frames Number of consecutive frames to allocate
 * @return Pointer to first frames address
 */
void *frame_alloc_consec(u8 n_frames);

/**
 * Free consecutively-allocated frames from its base address
 *
 * @param addr Physical memory location of frame
 * @param n_frames Number of frames to free
 */
void frame_free_consec(void *addr, u8 n_frames);

#endif

