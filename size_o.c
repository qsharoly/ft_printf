#include "libftprintf.h"

void	hho(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned char	nb;

	nb = (unsigned char)va_arg(ap, unsigned int);
	octal_to_string(str, prefix, f, nb);
}

void	ho(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned short	nb;

	nb = (unsigned short)va_arg(ap, unsigned int);
	octal_to_string(str, prefix, f, nb);
}

void	o(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned int	nb;

	nb = va_arg(ap, unsigned int);
	octal_to_string(str, prefix, f, nb);
}

void	lo(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	nb;

	nb = va_arg(ap, unsigned long);
	octal_to_string(str, prefix, f, nb);
}

void	llo(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long long	nb;

	nb = va_arg(ap, unsigned long long);
	octal_to_string(str, prefix, f, nb);
}
