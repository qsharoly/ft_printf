#include "libftprintf.h"

void hhd(char **str, char **prefix, t_fmt f, va_list ap)
{
	char	nb;

	nb = (char)va_arg(ap, int);
	signed_to_string(str, prefix, f, nb);
}

void hd(char **str, char **prefix, t_fmt f, va_list ap)
{
	short	nb;

	nb = (short)va_arg(ap, int);
	signed_to_string(str, prefix, f, nb);
}

void d(char **str, char **prefix, t_fmt f, va_list ap)
{
	int		nb;

	nb = va_arg(ap, int);
	signed_to_string(str, prefix, f, nb);
}

void ld(char **str, char **prefix, t_fmt f, va_list ap)
{
	long	nb;

	nb = va_arg(ap, long);
	signed_to_string(str, prefix, f, nb);
}

void	lld(char **str, char **prefix, t_fmt f, va_list ap)
{
	long long	nb;

	nb = va_arg(ap, long long);
	signed_to_string(str, prefix, f, nb);
}
