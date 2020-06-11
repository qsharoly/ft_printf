/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_diuox_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:55:31 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/26 00:17:39 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

#define LOWCASE 0
#define UPCASE 1

void	pf_putnbr(t_buffer *out, const char *value_start, const char *prefix,
			const t_fmt *fmt)
{
	int				value_len;
	int				pre_len;
	int				zero_fill;
	int				pad_len;

	value_len = ft_strlen(value_start);
	pre_len = ft_strlen(prefix);
	if (fmt->pad_with_zero && !fmt->left_justify && !fmt->has_precision)
		zero_fill = ft_imax(0, fmt->min_width - value_len - pre_len);
	else
		zero_fill = ft_imax(0, fmt->precision - value_len);
	pad_len = fmt->min_width - pre_len - zero_fill - value_len;
	while (!fmt->left_justify && (pad_len-- > 0))
		pf_putc(fmt->padchar, out);
	if (pre_len)
		pf_puts(prefix, out);
	while (zero_fill-- > 0)
		pf_putc('0', out);
	pf_puts(value_start, out);
	while (fmt->left_justify && (pad_len-- > 0))
		pf_putc(fmt->padchar, out);
}

void	p_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char			str[MAXBUF_ITOA];
	unsigned long	adr;
	const char		*value_start;

	adr = (unsigned long)va_arg(ap, void *);
#if __linux__
	if (adr == 0)
	{
		pf_putnbr(out, "(nil)", "", fmt);
		return ;
	}
#endif
	value_start = pf_utoa_base(str, adr, 16, LOWCASE);
	pf_putnbr(out, value_start, "0x", fmt);
}

static char	sign_prefix(long long nb, const t_fmt *fmt)
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

#define MAXBUF_SIGN_PREFIX 2
void		signed_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char			str[MAXBUF_ITOA];
	char			prefix[MAXBUF_SIGN_PREFIX];
	long long int	nb;
	const char		*value_start;

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
	if (nb < 0)
		value_start = pf_utoa_base(str, (unsigned long long)-nb, 10, LOWCASE);
	else
		value_start = pf_utoa_base(str, (unsigned long long)nb, 10, LOWCASE);
	ft_bzero(prefix, MAXBUF_SIGN_PREFIX);
	prefix[0] = sign_prefix(nb, fmt);
	pf_putnbr(out, value_start, prefix, fmt);
}

void		unsigned_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char				str[MAXBUF_ITOA];
	unsigned long long	nb;
	const char			*value_start;

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
	pf_putnbr(out, value_start, "", fmt);
}

void		octal_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char				str[MAXBUF_ITOA];
	unsigned long long	nb;
	const char			*value_start;

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
	if (fmt->alternative_form && ((int)ft_strlen(value_start) >= fmt->precision))
		pf_putnbr(out, value_start, "0", fmt);
	else
		pf_putnbr(out, value_start, "", fmt);
}

void		hex_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	char				str[MAXBUF_ITOA];
	unsigned long long	nb;
	const char			*value_start;
	const char			*prefix;
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
	upcase = (fmt->type == 'X');
	value_start = pf_utoa_base(str, nb, 16, upcase);
	if (fmt->alternative_form && nb > 0)
		prefix = upcase ? "0X" : "0x";
	else
		prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}
