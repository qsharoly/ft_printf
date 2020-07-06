/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_basic_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:23:11 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 19:48:08 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libftprintf.h"
#include "libft.h"

void	conv_default(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	(void)out;
	(void)f;
	(void)arg;
}

void	conv_percent(t_stream *out, t_fmt *f, union u_pfarg arg)
{
#if __APPLE__
	int		pad_len;

	(void)arg;
	pad_len = ft_imax(0, f->min_width - 1);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_putc('%', out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
#elif __linux__
	(void)arg;
	(void)f;
	pf_putc('%', out);
#endif
}

void	conv_c(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	int		pad_len;

	pad_len = ft_imax(0, f->min_width - 1);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_putc(arg.as_c, out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
}

void	conv_s(t_stream *out, t_fmt *f, union u_pfarg arg)
{
	int		value_len;
	int		pad_len;

	if (!arg.as_s)
		arg.as_s = "(null)";
	if (f->has_precision && f->precision >= 0)
		value_len = ft_imin(ft_strlen(arg.as_s), f->precision);
	else
		value_len = ft_strlen(arg.as_s);
	pad_len = ft_imax(0, f->min_width - value_len);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_nputs(arg.as_s, value_len, out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
}
