#include <io/vid.h>

#define KLOG_MAX_CHARS 1024

static enum klog_level min_level = KLOG_INIT;

int klog(enum klog_level lvl, const char *fmt, ...) {
    if(lvl >= min_level) {
        kprintf("[%d] ", lvl); // TODO: replace with PIT clock cycle

        __builtin_va_list varg;
        __builtin_va_start(varg, fmt);

        char tmp[KLOG_MAX_CHARS];
        int ret = fmt_print(tmp, fmt, varg);
        kputs(tmp);

        __builtin_va_end(varg);
        kputc('\n');
        return ret;
    }
    return 0;
}
