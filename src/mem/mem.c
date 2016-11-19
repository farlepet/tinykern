#include <io/vid.h>
#include <mem.h>

#ifdef DEBUG_MEMCHK
#define MEMCHK_NUM_PTRS 512
static void *ptrs[MEMCHK_NUM_PTRS];
static u32 n_ptrs = 0;

static int check_ptr(void *ptr, u32 size) {
    u32 i = 0;
    for(; i < n_ptrs; i++) {
        if(((u32)ptrs[i] + size) >= (u32)ptr && ptrs[i] <= ptr) {
            klog(KLOG_ERR, "check_ptr: Checked pointer found: %08x -> At %08x", ptrs[i], ptr);
            return 1;
        }
    }
    return 0;
}

void mem_add_check(void *ptr) {
    if(n_ptrs == MEMCHK_NUM_PTRS) {
        klog(KLOG_ERR, "mem_add_check: too many checks added!");
        return;
    }
    ptrs[n_ptrs] = ptr;
    n_ptrs++;
}
#endif

void *memcpy(void *dest, const void *src, u32 n) {
#ifdef DEBUG_MEMCHK
    check_ptr(dest, n);
#endif

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
#ifdef DEBUG_MEMCHK
    check_ptr(s, n);
#endif

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
