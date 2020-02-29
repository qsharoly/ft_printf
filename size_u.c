#include "libftprintf.h"

void hhu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned char	nb;

	nb = (unsigned char)va_arg(ap, unsigned int);
	unsigned_to_string(str, prefix, f, nb);
}

void hu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned short	nb;

	nb = (unsigned short)va_arg(ap, unsigned int);
	unsigned_to_string(str, prefix, f, nb);
}

void u(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned int	nb;

	nb = va_arg(ap, unsigned int);
	unsigned_to_string(str, prefix, f, nb);
}

void lu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	nb;

	nb = va_arg(ap, unsigned long);
	unsigned_to_string(str, prefix, f, nb);
}

void llu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long long	nb;

	nb = va_arg(ap, unsigned long long);
	unsigned_to_string(str, prefix, f, nb);
}
