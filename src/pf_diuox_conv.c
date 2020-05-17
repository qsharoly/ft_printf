/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_diuox_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:55:31 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/17 11:37:35 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

#define LOWCASE 0
#define UPCASE 1

/*
** default arg length is [unsigned] int
*/

static char	choose_sign_prefix(long long nb, const t_fmt *fmt)
{
	if (nb < 0)
		return ('-');
	else if (fmt->prepend_plus)
		return ('+');
	else if (fmt->prepend_space)
		return (' ');
	else
		return (0);
}

void	p_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char			str[MAXBUF_ITOA];
	char			*value_start;
	unsigned long	adr;
	char			*prefix;
	int				value_len;
	int				pad_len;
	int				pre_len;
	int				min_size;

	adr = (unsigned long)va_arg(ap, void *);
	value_start = pf_utoa_base(str, adr, 16, LOWCASE);
	prefix = "0x";
	pre_len = ft_strlen(prefix);
	value_len = ft_strlen(value_start);
	min_size = ft_imax(value_len, fmt->precision);
	pad_len = ft_imax(0, fmt->min_width - min_size - pre_len);
	while (!fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
	pf_puts(prefix, out);
	while (min_size-- > value_len)
		pf_putc('0', out);
	pf_puts(value_start, out);
	while (fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
}

void		signed_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char			str[MAXBUF_ITOA];
	char			*value_start;
	long long int	nb;
	int				min_size;
	int				value_len;
	int				pad_len;
	char			sign_prefix;

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
	if ((nb < 0 || fmt->prepend_plus || fmt->prepend_space)
			&& fmt->pad_with_zero && !fmt->left_justify && !fmt->has_precision)
		min_size = fmt->min_width - 1;
	else
		min_size = fmt->precision;
	if (nb < 0)
		value_start = pf_utoa_base(str, (unsigned long long)-nb, 10, LOWCASE);
	else
		value_start = pf_utoa_base(str, (unsigned long long)nb, 10, LOWCASE);
	value_len = ft_strlen(value_start);
	min_size = ft_imax(value_len, min_size);
	sign_prefix = choose_sign_prefix(nb, fmt);
	pad_len = ft_imax(0, fmt->min_width - min_size - (sign_prefix != 0));
	while (!fmt->left_justify && (pad_len-- > 0))
		pf_putc(fmt->padchar, out);
	if (sign_prefix)
		pf_putc(sign_prefix, out);
	while (min_size-- > value_len)
		pf_putc('0', out);
	pf_puts(value_start, out);
	while (fmt->left_justify && (pad_len-- > 0))
		pf_putc(fmt->padchar, out);
}

void		unsigned_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char				str[MAXBUF_ITOA];
	unsigned long long	nb;
	char				*value_start;
	int					value_len;
	int					min_size;
	int					pad_len;

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
	value_start = pf_utoa_base(str, nb, 10, 0);
	value_len = ft_strlen(value_start);
	min_size = ft_imax(value_len, fmt->precision);
	pad_len = ft_imax(0, fmt->min_width - min_size);
	while (!fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
	while (min_size-- > value_len)
		pf_putc('0', out);
	pf_puts(value_start, out);
	while (fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
}

void		octal_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char				str[MAXBUF_ITOA];
	unsigned long long	nb;
	char				*value_start;
	int					value_len;
	int					min_size;
	int					pad_len;
	int					pre_len;

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
	value_start = pf_utoa_base(str, nb, 8, 0);
	value_len = ft_strlen(value_start);
	pre_len = fmt->alternative_form;
	min_size = ft_imax(value_len, fmt->precision - pre_len);
	pad_len = ft_imax(0, fmt->min_width - min_size - pre_len);
	while (!fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
	if (pre_len)
		pf_putc('0', out);
	while (min_size-- > value_len)
		pf_putc('0', out);
	pf_puts(value_start, out);
	while (fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
}

#define HEX_PREFIX_LEN 2
void		hex_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char				str[MAXBUF_ITOA];
	unsigned long long	nb;
	int					min_size;
	char				*prefix;
	int					upcase;
	char				*value_start;
	int					value_len;
	int					pad_len;
	int					pre_len;

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
	upcase = (fmt->type == 'X');
	if (fmt->alternative_form && nb > 0)
		prefix = upcase ? "0X" : "0x";
	else
		prefix = NULL;
	pre_len = prefix ? ft_strlen(prefix) : 0;
	if (fmt->alternative_form && fmt->pad_with_zero && !fmt->left_justify
			&& fmt->min_width - pre_len > fmt->precision)
		min_size = fmt->min_width - pre_len;
	else
		min_size = fmt->precision;
	value_start = pf_utoa_base(str, nb, 16, upcase);
	value_len = ft_strlen(value_start);
	min_size = ft_imax(value_len, min_size);
	pad_len = ft_imax(0, fmt->min_width - min_size - pre_len);
	while (!fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
	if (prefix)
		pf_puts(prefix, out);
	while (min_size-- > value_len)
		pf_putc('0', out);
	pf_puts(value_start, out);
	while (fmt->left_justify && pad_len-- > 0)
		pf_putc(fmt->padchar, out);
}
