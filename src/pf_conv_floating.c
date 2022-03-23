/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_conv_floating.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2022/03/24 00:45:36 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "float.h"
#include <limits.h>

t_sv	sign_prefix(int is_negative, const t_fmt *fmt)
{
	if (is_negative)
		return (sv_from_cstr("-"));
	else if (fmt->plus_mode == ExplicitPlus)
		return (sv_from_cstr("+"));
	else if (fmt->plus_mode == ExplicitSpace)
		return (sv_from_cstr(" "));
	else
		return (sv_from_cstr(""));
}

static int	put_if_special(t_stream *out, t_fmt *fmt, long double nb)
{
	t_sv		value;
	t_sv		sign;
	int			pad_length;

	if (ft_isnan(nb))
		value = sv_from_cstr("nan");
	else if (ft_isinf(nb))
		value = sv_from_cstr("inf");
	else
		return (0);
	sign = sign_prefix(ft_isneg(nb), fmt);
	pad_length = fmt->min_width - (sign.length + value.length);
	put_repeat(' ', (fmt->align == AlignRight) * pad_length, out);
	put_sv(sign, out);
	put_sv(value, out);
	put_repeat(' ', (fmt->align == AlignLeft) * pad_length, out);
	return (1);
}

void		conv_floating(t_stream *out, t_fmt *fmt, va_list ap)
{
	long double	d;

	if (fmt->size == Size_longdouble)
		d = va_arg(ap, long double);
	else
		d = va_arg(ap, double);
	if (fmt->has_precision == 0)
		fmt->precision = DTOA_DEFAULT_PRECISION;
	if (put_if_special(out, fmt, d))
		return ;
	if (ft_fabs(d) < (long double)ULONG_MAX && fmt->precision < 20)
		pf_dtoa_quick(out, d, fmt);
	else
		pf_dtoa(out, d, fmt);
}
