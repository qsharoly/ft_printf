/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_conv_diuox.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:55:31 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/23 09:43:35 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

#if __linux__

void	conv_ptr(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_UTOA];
	const char		*value_start;

	if (!arg.as_ptr)
	{
		pf_putnbr(out, "(nil)", "", fmt);
		return ;
	}
	value_start = pf_utoa_base(str, (unsigned long)arg.as_ptr, 16, 0);
	pf_putnbr(out, value_start, "0x", fmt);
}

#else

void	conv_ptr(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_UTOA];
	const char		*value_start;

	value_start = pf_utoa_base(str, (unsigned long)arg.as_ptr, 16, 0);
	pf_putnbr(out, value_start, "0x", fmt);
}

#endif

void	conv_signed(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_UTOA];
	const char		*value_start;
	char			prefix[2];

	value_start = pf_utoa_base(str, ft_abs(arg.as_i), 10, 0);
	prefix[0] = sign_char(arg.as_i < 0, fmt);
	prefix[1] = '\0';
	pf_putnbr(out, value_start, prefix, fmt);
}

void	conv_unsigned(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_UTOA];
	const char		*value_start;
	const char		*prefix;

	value_start = pf_utoa_base(str, arg.as_u, 10, 0);
	prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}

void	conv_oct(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	char			str[MAXBUF_UTOA];
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
	char			str[MAXBUF_UTOA];
	const char		*value_start;
	const char		*prefix;

	value_start = pf_utoa_base(str, arg.as_u, 16, fmt->upcase);
	if (fmt->alternative_form && arg.as_u > 0)
		prefix = fmt->upcase ? "0X" : "0x";
	else
		prefix = "";
	pf_putnbr(out, value_start, prefix, fmt);
}
