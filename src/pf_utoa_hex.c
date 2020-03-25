#include <stdlib.h>

static int  nb_size_hex(unsigned long long value)
{
    int     size;
    
    size = 0;
    while (value)
    {
        value >>= 4;
        size++;
    }
    return (size);
}

static int  max(int a, int b)
{
    return (a > b ? a : b);
}

char    *pf_utoa_hex(unsigned long long int value, int min_digits, int prefix, int upper)
{
    char    *buf;
	char	*digits;
    int     end;
    int     i;

	digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    end = max(min_digits, nb_size_hex(value));
    end += 2 * prefix * (min_digits <= nb_size_hex(value));
    buf = malloc(sizeof(char) * (end + 1));
    buf[end] = '\0';
    end--;
    i = 0;
    while (value)
    {
        buf[end - i] = digits[value & 0xf];
        value >>= 4;
        i++;
    }
    while (i < min_digits)
    {
        buf[end - i] = '0';
        i++;
    }
    if (prefix)
	{
        buf[0] = '0';
		buf[1] = upper ? 'X' : 'x';
	}
    return (buf);
}
