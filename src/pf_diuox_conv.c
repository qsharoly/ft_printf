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
	*str = pf_utoa_oct(nb, fmt.precision, fmt.alternative_form);
	*prefix = NULL;
}

void hex_conv(char **str, char **prefix, t_fmt fmt, va_list ap)
{
	unsigned long long	nb;
	int	upcase;
	int	min_size;

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
	min_size = (fmt.alternative_form && fmt.pad_with_zero && !fmt.left_justify && fmt.min_field_width > fmt.precision) ? fmt.min_field_width : fmt.precision;
	*str = pf_utoa_hex(nb, min_size, fmt.alternative_form && nb > 0, upcase);
	*prefix = NULL;
}
