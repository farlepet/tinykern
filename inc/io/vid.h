#ifndef VID_H
#define VID_H

#include <io/serial.h>

#define FMT_SPEC '%'


enum klog_level {
    KLOG_DETAIL,
    KLOG_INFO,
    KLOG_INITINFO,
    KLOG_INIT,
    KLOG_WARN,
    KLOG_ERR,
    KLOG_FATAL
};


/**
 * Print a character to the selected output device
 *
 * @param ch Character to print
 */
void kputc(char ch);

/**
 * Print a string to the selected output device
 *
 * @param str String to print
 */
void kputs(char *str);

/**
 * Set the output serial port for kputc
 *
 * @param port Base IO address of serial port to use
 */
void video_set_serial_port(enum serial_port port);

/**
 * Takes a format string and a list of arguments as input, and produces a
 * string as output.
 *
 * @param out The output string
 * @param format The format string
 * @param varg The list of arguments
 * @return The number of charactern placed in `out`
 */
int fmt_print(char *out, const char *format, __builtin_va_list varg);

/**
 * Uses `fmt_print` to convert the format string and any number of arguments to
 * an output string.
 *
 * @param out output string
 * @param fmt format string
 * @param ... argument list
 * @return the number of characters placed in `out`
 */
int sprintf(char *out, const char *fmt, ...);

/**
 * Uses `fmt_print` to convert the format string and any number of arguments to
 * a string then prints that string to the currently selected output.
 *
 * @param fmt format string
 * @param ... argument list
 * @return the number of characters printed
 */
int kprintf(const char *fmt, ...);

/**
 * If lvl is greater than or equal to the current log level, it prints a formatted string
 *
 * @param lvl message level
 * @param fmt format string
 * @param ... argument list
 */
int klog(enum klog_level lvl, const char *fmt, ...);

#endif
