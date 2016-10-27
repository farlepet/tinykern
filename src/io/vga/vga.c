#include <mem.h>
#include <types.h>
#include <io/vga/vga.h>

static u8 *vgamem = (u8 *)0xB8000;

static u8  vga_color = 0x0F;
static u16 vga_cols = 80, vga_lines = 25;
static u16 vga_cur_col = 0, vga_cur_line = 0;

void vga_scrollup() {
    memcpy(vgamem, vgamem + vga_cols * 2, (vga_cols * (vga_lines - 1)) * 2); // Move text up
    memset(vgamem + (vga_cols * (vga_lines - 1)) * 2, 0, vga_cols * 2);       // Clear bottom row
}

void vga_putch(char ch) {
    if(ch == '\n') { // Newline
        vga_cur_col = 0;
        vga_cur_line++;
    } else { // Other character
        u16 offset = ((vga_cur_line * vga_cols) + vga_cur_col) * 2;
        vgamem[offset] = (u8)ch;
        vgamem[offset + 1] = vga_color;
        vga_cur_col++;
    }
    if(vga_cur_col >= vga_cols) { // We're at the edge of the display
        vga_cur_col = 0;
        vga_cur_line++;
    }
    if(vga_cur_line >= vga_lines) { // We're at the bottom of the display
        vga_scrollup();
    }
}

void vga_puts(char *str) {
    while(*str) {
        vga_putch(*str);
        str++;
    }
}

void vga_clear() {
    memset(vgamem, 0, vga_cols * vga_lines * 2);
}
