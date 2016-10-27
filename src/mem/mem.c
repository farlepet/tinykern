#include <mem.h>


void *memcpy(void *dest, const void *src, u32 n) {
    u32 *d_32 = (u32 *)dest;
    const u32 *s_32 = (const u32 *)src;
    u8  *d_8  = (u8 *)dest;
    const u8 *s_8  = (const u8 *)src;

    while(n >= 4) {
        *d_32 = *s_32;
        d_32 += 4; d_32 += 4; n -= 4;
    }
    while(n) {
        *d_8 = *s_8;
        d_8++; s_8++; n--;
    }
    return dest;
}

void *memset(void *s, int c, u32 n) {
    u32 v_32 = (u32)(c | c << 8 | c << 16 | c << 24);
    u32 *s_32 = (u32 *)s;
    u8  *s_8  = (u8  *)s;
    while(n >= 4) {
        *s_32 = v_32;
        s_32++; n -= 4;
    }
    while(n) {
        *s_8 = (u8)c;
        s_32++; n--;
    }
    return s;
}
