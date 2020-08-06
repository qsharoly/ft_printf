/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_diuox_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:55:31 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:19:18 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

#if __linux__

void	conv_p(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	unsigned long	adr;
	const char		*value_start;

	adr = (unsigned long)arg.as_ptr;
	if (adr == 0)
	{
		pf_putnbr(out, "(nil)", "", fmt);
		return ;
	}
	value_start = pf_utoa_base(str, adr, 16, 0);
	pf_putnbr(out, value_start, "0x", fmt);
}

#else

void	conv_p(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	unsigned long	adr;
	const char		*value_start;

	adr = (unsigned long)arg.as_ptr;
	value_start = pf_utoa_base(str, adr, 16, 0);
	pf_putnbr(out, value_start, "0x", fmt);
}

#endif

void	conv_signed(t_stream *out, t_fmt *fmt, union u_pfarg arg)
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

void	conv_unsigned(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	const char		*value_start;
	const char		*prefix;

	value_start = pf_utoa_base(str, arg.as_u, 10, 0);
	prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}

void	conv_oct(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	const char		*value_start;
	const char		*prefix;

	value_start = pf_utoa_base(str, arg.as_u, 8, 0);
	if (fmt->alternative_form
		&& ((int)ft_strlen(value_start) >= fmt->precision))
		prefix = "0";
	else
		prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}

void	conv_hex(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_ITOA];
	const char		*value_start;
	const char		*prefix;
	int				upcase;

	upcase = (fmt->conv == Conv_hex_uppercase);
	value_start = pf_utoa_base(str, arg.as_u, 16, upcase);
	if (fmt->alternative_form && arg.as_u > 0)
		prefix = upcase ? "0X" : "0x";
	else
		prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}
