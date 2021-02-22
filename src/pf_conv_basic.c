/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_conv_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:23:11 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/23 01:45:12 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libftprintf.h"
#include "libft.h"

void	conv_default_noop(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	(void)out;
	(void)f;
	(void)arg;
}

#if __APPLE__

void	conv_percent(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	int		pad_len;

	(void)arg;
	pad_len = ft_imax(0, f->min_width - 1);
	pf_repeat(f->padchar, !f->left_justify * pad_len, out);
	pf_putc('%', out);
	pf_repeat(f->padchar, f->left_justify * pad_len, out);
}

#elif __linux__

void	conv_percent(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	(void)arg;
	(void)f;
	pf_putc('%', out);
}

#endif

void	conv_char(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	int		pad_len;

	pad_len = ft_max(0, f->min_width - 1);
	pf_repeat(f->padchar, !f->left_justify * pad_len, out);
	pf_putc(arg.as_c, out);
	pf_repeat(f->padchar, f->left_justify * pad_len, out);
}

void	conv_str(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	int		value_len;
	int		pad_len;

	if (!arg.as_s)
	{
		if (f->has_precision && f->precision < 6)
			arg.as_s = "";
		else
			arg.as_s = "(null)";
	}
	if (f->has_precision && f->precision >= 0)
		value_len = ft_min(ft_strlen(arg.as_s), f->precision);
	else
		value_len = ft_strlen(arg.as_s);
	pad_len = ft_max(0, f->min_width - value_len);
	pf_repeat(f->padchar, !f->left_justify * pad_len, out);
	pf_nputs(arg.as_s, value_len, out);
	pf_repeat(f->padchar, f->left_justify * pad_len, out);
}
