#ifndef MEM_H
#define MEM_H

#include <types.h>

/**
 * Copy data from one location to another
 *
 * @param dest Destination pointer
 * @param src Source pointer
 * @param n Number of bytes to copy
 */
void *memcpy(void *dest, const void *src, u32 n);

/**
 * Set a string of data to a specified value
 *
 * @param s Location to act upon
 * @param c 8-bit value to set data to
 * @param n Number of bytes to set
 */
void *memset(void *s, int c, u32 n);

#ifdef DEBUG_MEMCHK
// Add pointer to list to check at every call of mem___
void mem_add_check(void *ptr);
#else
#define mem_add_check(p)
#endif

#endif
