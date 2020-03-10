#include "libft.h"
#include "libftprintf.h"

void signed_conv(char **str, char **prefix, t_fmt fmt, va_list ap)
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
	*str = pf_itoa_base_abs(nb, 10, fmt.precision, 0);
	if (nb >= 0)
	{
		if (fmt.prepend_plus)
			*prefix = pf_strclone("+");
		else if (fmt.prepend_space)
			*prefix = pf_strclone(" ");
		else
			*prefix = NULL;
	}
	else
		*prefix = pf_strclone("-");
}

void	unsigned_conv(char **str, char **prefix, t_fmt fmt, va_list ap)
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
	*str = pf_utoa_base(nb, 10, fmt.precision, 0);
	*prefix = NULL;
}

void	octal_conv(char **str, char **prefix, t_fmt fmt, va_list ap)
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
	*str = pf_utoa_base(nb, 8, fmt.precision, 0);
	if (fmt.alternative_form && nb != 0 && fmt.precision <= 1)
		*prefix = pf_strclone("0");
	else
		*prefix = NULL;
}

void hex_conv(char **str, char **prefix, t_fmt fmt, va_list ap)
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
	*str = pf_utoa_base(nb, 16, fmt.precision, upcase);
	if (fmt.alternative_form && nb != 0)
		*prefix = upcase ? pf_strclone("0X") : pf_strclone("0x");
	else
		*prefix = NULL;
}
