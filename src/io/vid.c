#include <io/vga/vga.h>
#include <io/vid.h>

void kputc(char ch) {
    vga_putch(ch);
}

void kputs(char *str) {
    while(*str) kputc(*(str++));
}
