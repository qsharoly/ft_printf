/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_diuox_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:55:31 by qsharoly          #+#    #+#             */
/*   Updated: 2020/04/14 14:38:06 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

/*
** default arg length is [unsigned] int
*/

static char	sign_prefix(int is_negative, const t_fmt *fmt)
{
	if (is_negative)
		return ('-');
	else if (fmt->prepend_plus)
		return ('+');
	else if (fmt->prepend_space)
		return (' ');
	else
		return (0);
}

void		signed_conv(char **str, const t_fmt *fmt, va_list ap)
{
	long long int	nb;
	int				min_size;
	int				prefix_size;

	if (fmt->is_char)
		nb = (char)va_arg(ap, int);
	else if (fmt->is_short)
		nb = (short)va_arg(ap, int);
	else if (fmt->is_long)
		nb = va_arg(ap, long);
	else if (fmt->is_longlong)
		nb = va_arg(ap, long long);
	else
		nb = va_arg(ap, int);
	prefix_size = 1;
	if ((nb < 0 || fmt->prepend_plus || fmt->prepend_space)
			&& fmt->pad_with_zero && !fmt->left_justify && !fmt->has_precision)
		min_size = fmt->min_width - prefix_size;
	else
		min_size = fmt->precision;
	*str = pf_itoa_dec(nb, min_size, sign_prefix(nb < 0, fmt));
}

void		unsigned_conv(char **str, const t_fmt *fmt, va_list ap)
{
	unsigned long long nb;

	if (fmt->is_char)
		nb = (unsigned char)va_arg(ap, unsigned int);
	else if (fmt->is_short)
		nb = (unsigned short)va_arg(ap, unsigned int);
	else if (fmt->is_long)
		nb = va_arg(ap, unsigned long);
	else if (fmt->is_longlong)
		nb = va_arg(ap, unsigned long long);
	else
		nb = va_arg(ap, unsigned int);
	*str = pf_utoa_base(nb, 10, fmt->precision, 0);
}

void		octal_conv(char **str, const t_fmt *fmt, va_list ap)
{
	unsigned long long	nb;
	int					min_size;
	int					has_prefix;

	if (fmt->is_char)
		nb = (unsigned char)va_arg(ap, unsigned int);
	else if (fmt->is_short)
		nb = (unsigned short)va_arg(ap, unsigned int);
	else if (fmt->is_long)
		nb = va_arg(ap, unsigned long);
	else if (fmt->is_longlong)
		nb = va_arg(ap, unsigned long long);
	else
		nb = va_arg(ap, unsigned int);
	min_size = fmt->precision;
	has_prefix = fmt->alternative_form;
	*str = pf_utoa_oct(nb, min_size, has_prefix);
}

/*
** hex_conv:
*/

void		hex_conv(char **str, const t_fmt *fmt, va_list ap)
{
	unsigned long long	nb;
	int					min_size;
	int					has_prefix;
	int					prefix_size;
	int					upcase;

	if (fmt->is_char)
		nb = (unsigned char)va_arg(ap, unsigned int);
	else if (fmt->is_short)
		nb = (unsigned short)va_arg(ap, unsigned int);
	else if (fmt->is_long)
		nb = va_arg(ap, unsigned long);
	else if (fmt->is_longlong)
		nb = va_arg(ap, unsigned long long);
	else
		nb = va_arg(ap, unsigned int);
	prefix_size = 2;
	if (fmt->alternative_form && fmt->pad_with_zero && !fmt->left_justify
			&& fmt->min_width - prefix_size > fmt->precision)
		min_size = fmt->min_width - prefix_size;
	else
		min_size = fmt->precision;
	has_prefix = (fmt->alternative_form && nb > 0);
	upcase = (fmt->type == 'X');
	*str = pf_utoa_hex(nb, min_size, has_prefix, upcase);
}
