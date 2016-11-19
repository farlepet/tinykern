#include <mem/paging.h>
#include <string.h>
#include <io/vid.h>

int strlen(char *str) {
    int i = 0;
    while(str[i]) i++;
    return i;
}
