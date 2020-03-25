#include <stdlib.h>
#include "libftprintf.h"

static int  nb_size_oct(unsigned long long value)
{
    int     size;
    
    size = 0;
    while (value)
    {
        value >>= 3;
        size++;
    }
    return (size);
}

char    *pf_utoa_oct(unsigned long long int value, int min_digits, int prefix)
{
    char    *buf;
    int     end;
    int     i;

    end = pf_max(min_digits, nb_size_oct(value));
    end += prefix * (min_digits <= nb_size_oct(value));
    buf = malloc(sizeof(char) * (end + 1));
    buf[end] = '\0';
    end--;
    i = 0;
    while (value)
    {
        buf[end - i] = '0' + (value & 7);
        value >>= 3;
        i++;
    }
    while (i < min_digits)
    {
        buf[end - i] = '0';
        i++;
    }
    if (prefix)
        buf[0] = '0';
    return (buf);
}
