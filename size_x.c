#include "libftprintf.h"

void	hhx(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned char	nb;

	nb = (unsigned char)va_arg(ap, unsigned int);
	hex_to_string(str, prefix, f, nb);
}

void	hx(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned short	nb;

	nb = (unsigned short)va_arg(ap, unsigned int);
	hex_to_string(str, prefix, f, nb);
}

void	x(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned int	nb;

	nb = va_arg(ap, unsigned int);
	hex_to_string(str, prefix, f, nb);
}

void	lx(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	nb;

	nb = va_arg(ap, unsigned long);
	hex_to_string(str, prefix, f, nb);
}

void	llx(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long long	nb;

	nb = va_arg(ap, unsigned long long);
	hex_to_string(str, prefix, f, nb);
}
