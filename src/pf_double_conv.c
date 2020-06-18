/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_double_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 16:00:50 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "float.h"

void		double_conv(t_buffer *out, t_fmt *fmt, va_list ap)
{
	long double		l;
	double			d;

	if (fmt->is_quad)
		l = va_arg(ap, long double);
	else
		d = va_arg(ap, double);
	if (fmt->has_precision == 0)
		fmt->precision = DTOA_DEFAULT_PRECISION;
	if (fmt->is_quad)
		pf_ldtoa(out, l, fmt);
	else
		pf_dtoa(out, d, fmt);
}
