#ifndef STRING_H
#define STRING_H

/**
 * Get the length of a string
 *
 * @param str The string
 * @return The length of the string
 */
int strlen(char *str);

/**
 * Copy one string into another
 *
 * @param dest Destination string
 * @param src Source string
 * @return Pointer to destination string
 */
char *strcpy(char *dest, const char *src);

#endif
