/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_diuox_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:55:31 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 16:00:00 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

void	pf_putnbr(t_stream *out, const char *value_start, const char *prefix,
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

void	conv_p(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	unsigned long	adr;
	const char		*value_start;

	adr = (unsigned long)arg.as_ptr;
#if __linux__
	if (adr == 0)
	{
		pf_putnbr(out, "(nil)", "", fmt);
		return ;
	}
#endif
	value_start = pf_utoa_base(str, adr, 16, 0);
	pf_putnbr(out, value_start, "0x", fmt);
}

void		conv_signed(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	const char		*value_start;
	const char		*prefix;

	if (arg.as_i < 0)
		value_start = pf_utoa_base(str, -arg.as_i, 10, 0);
	else
		value_start = pf_utoa_base(str, arg.as_i, 10, 0);
	if (arg.as_i < 0)
		prefix = "-";
	else if (fmt->prepend_plus)
		prefix = "+";
	else if (fmt->prepend_space)
		prefix = " ";
	else
		prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}

void		conv_unsigned(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	const char		*value_start;
	const char		*prefix;

	value_start = pf_utoa_base(str, arg.as_u, 10, 0);
	prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}

void		conv_oct(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char				str[MAXBUF_ITOA];
	const char			*value_start;
	const char			*prefix;

	value_start = pf_utoa_base(str, arg.as_u, 8, 0);
	if (fmt->alternative_form && ((int)ft_strlen(value_start) >= fmt->precision))
		prefix = "0";
	else
		prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}

void		conv_hex(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char				str[MAXBUF_ITOA];
	const char			*value_start;
	const char			*prefix;
	int					upcase;

	upcase = (fmt->type == Type_hex_uppercase);
	value_start = pf_utoa_base(str, arg.as_u, 16, upcase);
	if (fmt->alternative_form && arg.as_u > 0)
		prefix = upcase ? "0X" : "0x";
	else
		prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}
