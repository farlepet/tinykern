#include <io/vga/vga.h>
#include <io/serial.h>
#include <io/vid.h>

static enum serial_port vid_ser_out = SER_NULL;

void kputc(char ch) {
    if(vid_ser_out) {
        serial_putc(vid_ser_out, ch);
    } else {
        vga_putch(ch);
    }
}

void kputs(char *str) {
    while(*str) kputc(*(str++));
}

void video_set_serial_port(enum serial_port port) {
    vid_ser_out = port;
}
