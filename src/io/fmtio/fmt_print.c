#include <io/vid.h>
#include <string.h>
#include <types.h>


/**
 * Converts an interger into a string and places the output into `out`
 * See `fmt_print`
 *
 * @param num the number to print
 * @param base the base in which to print the number
 * @param u wether the number is unsigned
 * @param pad how many characters of padding to give the number
 * @param padzero wether or not to pad the number with zeros instead of spaces
 * @param possign wether or not to place a positive sign in front of positive numbers
 * @param posspace wether or not to place a space in front of positive numbers
 * @param _case upper or lowercase (for bases above 10)
 * @param out the output string/buffer
 * @return the number of characters that have been put in `out`
 * */
static int print_int(u32 num, u8 base, u8 u, u8 pad, u8 padzero, u8 possign, u8 posspace, u8 _case, char *out)
{
    int onum = (int)num;
    if(onum < 0) if(!u) num = (~num) + 1;
    char *nums;
    if(_case) nums = "0123456789ABCDEF";
    else nums = "0123456789abcdef";
    char ans[16] = { '0', };
    int i = 0;
    while(num)
    {
        ans[i++] = nums[num % base];
        num /= base;
    }
    if(i == 0) i++;
    if(!u)
    {
        if(onum >= 0)
        {
            if(possign)
            {
                *out++ = '+';
            }
            else if(posspace)
                *out++ = ' ';
        }
        else if(onum < 0)
            *out++ = '-';
    }
    int p = pad - i;
    if(p > 0)
    {
        while(p--) *out++ = (padzero ? '0' : ' ');
        while(--i >= 0) *out++ = ans[i];
        return (int)((pad > strlen(ans)) ? pad : strlen(ans)) + ((((possign || posspace) && !u) && onum >= 0) || ((onum < 0) && !u)) + 1;
    }
    else
    {
        while(--i >= 0) *out++ = ans[i];
        return (int)(strlen(ans) + ((((possign || posspace) && !u) && onum >= 0) || ((onum < 0) && !u)));
    }
}

/**
 * Convert a number in a string into an integer. See `fmt_print`
 *
 * @param str input string
 * @param out pointer to end of number (generated by this function)
 * @return the number found
 */
static int get_dec(const char *str, ptr_t *out)
{
    int n = 0;
    while(*str >= '0' && *str <= '9')
    {
        n *= 10;
        n += (int)(*str - '0');
        str++;
    }
    *out = (ptr_t)str;
    return n;
}

#define ZERO_ALL_VID() \
    do { \
        is_in_spec = 0; \
        size = 0; \
        width = 0; \
        precision = 0; \
        showsign = 0; \
        signspace = 0; \
        leftalign = 0; \
        padzeros = 0; \
    } while(0);


#define va_arg __builtin_va_arg

int fmt_print(char *out, const char *format, __builtin_va_list varg) {
	u8 is_in_spec = 0;
    s8 size = 0;      // Size of the integer
    u8 width = 0;     // Width of the number at minimum
    u8 precision = 0; // Precision
    u8 showsign = 0;  // Show the sign on positive numbers
    u8 signspace = 0; // Place a space before positive numbers
    u8 leftalign = 0; // Align to the left
    u8 padzeros = 0;  // Use zeros instead of spaces for padding

    int nchars = 0;   // Number of chars printed so far
    ptr_t temp;
    
    for(; *format != 0; format++) {
        if(!is_in_spec) {
            if(*format == FMT_SPEC) {
                is_in_spec = 1;
                continue;
            }
            *out++ = *format;
            nchars++;
            continue;
        }
        switch(*format) {
            case FMT_SPEC:
                is_in_spec = 0;
                *out++ = FMT_SPEC;
                nchars++;
                break;
            
            case 'l':
                if(size < 2) size++;
                break;

            case 'L':
                size = 1;
                break;

            case 'h':
                if(size > -2) size--;
                break;

            case 'z':
            case 'j':
            case 't':
                size = 0;
                break;

            case '+':
                showsign = 1;
                break;

            case ' ':
                signspace = 1;
                break;

            case '-':
                leftalign = 1;
                break;

            case '0':
                padzeros = 1;
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                width = (u8)get_dec(format, &temp);
                format = (char *)(temp - 1);
                break;

            case '.':
                format++;
                precision = (u8)get_dec(format, &temp);
                format = (char *)(temp - 1);
                break;

            // Numbers, strings, etc...
            case 'd':
            case 'i':
                temp = (ptr_t)va_arg(varg, int);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (ptr_t)print_int(temp, 10, 0, width, padzeros, showsign, signspace, 0, out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 'u':
                temp = (ptr_t)va_arg(varg, u32);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (ptr_t)print_int(temp, 10, 1, width, padzeros, showsign, signspace, 0, out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                (void)va_arg(varg, double);
                ZERO_ALL_VID();
                break;
            
            case 'x':
            case 'X': 
                temp = (ptr_t)va_arg(varg, u32);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (ptr_t)print_int(temp, 16, 1, width, padzeros, showsign, signspace, (*format == 'X'), out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 'o':
                temp = (ptr_t)va_arg(varg, u32);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (ptr_t)print_int(temp, 8, 1, width, padzeros, showsign, signspace, 0, out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 's':
                if(size > 0) {
                    // NOTE: Not supported!
                } else {
                    temp = (ptr_t)va_arg(varg, char *);
                    nchars += strlen((char *)temp);
                    while(*(char *)temp) *out++ = *(char *)temp++;
                }
                ZERO_ALL_VID();
                break;

            case 'c':
                temp = (ptr_t)va_arg(varg, int);
                if(size > 0) *out++ = (char)temp;
                else *out++ = (char)temp;
                nchars++;
                ZERO_ALL_VID();
                break;

            case 'p':
                temp = (ptr_t)va_arg(varg, ptr_t);
                temp = (ptr_t)print_int(temp, 16, 1, width, padzeros, showsign, signspace, 1 /* Should this be upper or lower 
                case? */, out);
                nchars += temp;
                out += temp;
                break;

            case 'a':
            case 'A':
                (void)va_arg(varg, double);
                ZERO_ALL_VID();
                break;

            case 'n':
                (void)va_arg(varg, int);
                ZERO_ALL_VID();
                break;
        }
    }

    *(out) = 0;
    // These aren't used yet:
    (void)precision;
    (void)leftalign;
    return nchars;
}
