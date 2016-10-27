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

#endif
