#include <mem.h>
#include <io/vid.h>

int sprintf(char *out, const char *fmt, ...) {
    __builtin_va_list varg;
    __builtin_va_start(varg, fmt);
    
    int ret = fmt_print(out, fmt, varg);
    
    __builtin_va_end(varg);
    return ret;
}
