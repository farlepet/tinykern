#ifndef IO_VGA_H
#define IO_VGA_H

/**
 * Scrolls the VGA text buffer up by one line
 */
void vga_scrollup(void);

/**
 * Prints a character to the VGA text buffer
 *
 * @param ch Character to print
 */
void vga_putch(char ch);

/**
 * Prints a string to the VGA text buffer (see: vga_putch)
 *
 * @param str String to print
 */
void vga_puts(char *str);

/**
 * Clears the VGA text buffer
 */
void vga_clear(void);

#endif
