#include "libftprintf.h"

void signed_to_string(char **str, char **prefix, t_fmt fmt, va_list ap)
{
	long long int	nb;

	if (fmt.is_char)
		nb = (char)va_arg(ap, int);
	else if (fmt.is_short)
		nb = (short)va_arg(ap, int);
	else if (fmt.is_int)
		nb = va_arg(ap, int);
	else if (fmt.is_long)
		nb = va_arg(ap, long);
	else// if (fmt.is_longlong)
		nb = va_arg(ap, long long);
	*str = ft_itoa_base_abs(nb, 10, fmt.precision, 0);
	if (nb >= 0)
	{
		if (fmt.prepend_plus)
			*prefix = ft_strclone("+");
		else if (fmt.prepend_space)
			*prefix = ft_strclone(" ");
		else
			*prefix = NULL;
	}
	else
		*prefix = ft_strclone("-");
}

void	unsigned_to_string(char **str, char **prefix, t_fmt fmt, va_list ap)
{
	unsigned long long nb;

	if (fmt.is_char)
		nb = (unsigned char)va_arg(ap, unsigned int);
	else if (fmt.is_short)
		nb = (unsigned short)va_arg(ap, unsigned int);
	else if (fmt.is_int)
		nb = va_arg(ap, unsigned int);
	else if (fmt.is_long)
		nb = va_arg(ap, unsigned long);
	else //if (fmt.is_longlong)
		nb = va_arg(ap, unsigned long long);
	*str = ft_utoa_base(nb, 10, fmt.precision, 0);
	*prefix = NULL;
}

void	octal_to_string(char **str, char **prefix, t_fmt fmt, va_list ap)
{
	unsigned long long	nb;

	if (fmt.is_char)
		nb = (unsigned char)va_arg(ap, unsigned int);
	else if (fmt.is_short)
		nb = (unsigned short)va_arg(ap, unsigned int);
	else if (fmt.is_int)
		nb = va_arg(ap, unsigned int);
	else if (fmt.is_long)
		nb = va_arg(ap, unsigned long);
	else// if (fmt.is_longlong)
		nb = va_arg(ap, unsigned long long);
	if (fmt.alternative_form && fmt.precision == 0 && nb == 0)
		fmt.precision++;
	*str = ft_utoa_base(nb, 8, fmt.precision, 0);
	if (fmt.alternative_form && nb != 0 && fmt.precision <= 1)
		*prefix = ft_strclone("0");
	else
		*prefix = NULL;
}

void hex_to_string(char **str, char **prefix, t_fmt fmt, va_list ap)
{
	unsigned long long	nb;
	int	upcase;

	if (fmt.is_char)
		nb = (unsigned char)va_arg(ap, unsigned int);
	else if (fmt.is_short)
		nb = (unsigned short)va_arg(ap, unsigned int);
	else if (fmt.is_int)
		nb = va_arg(ap, unsigned int);
	else if (fmt.is_long)
		nb = va_arg(ap, unsigned long);
	else //if (fmt.is_longlong)
		nb = va_arg(ap, unsigned long long);
	upcase = (fmt.type == 'X');
	*str = ft_utoa_base(nb, 16, fmt.precision, upcase);
	if (fmt.alternative_form && nb != 0)
		*prefix = upcase ? ft_strclone("0X") : ft_strclone("0x");
	else
		*prefix = NULL;
}
