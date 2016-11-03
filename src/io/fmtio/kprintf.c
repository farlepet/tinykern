#include <mem.h>
#include <io/vid.h>

#define KPRINTF_MAX_CHARS 1024

int kprintf(const char *fmt, ...) {
    __builtin_va_list varg;
    __builtin_va_start(varg, fmt);

    char tmp[KPRINTF_MAX_CHARS];
    memset(tmp, ' ', KPRINTF_MAX_CHARS);
    int ret = fmt_print(tmp, fmt, varg);
    kputs(tmp);

    __builtin_va_end(varg);
    return ret;
}
