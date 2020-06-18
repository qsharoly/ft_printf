/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_double_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 21:54:06 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "float.h"
#include <limits.h>

void		double_conv(t_buffer *out, t_fmt *fmt, va_list ap)
{
	long double		l;
	double			d;

	if (fmt->has_precision == 0)
		fmt->precision = DTOA_DEFAULT_PRECISION;
	if (fmt->is_quad)
	{
		l = va_arg(ap, long double);
		if (ft_fabsl(l) < (long double)ULONG_MAX && fmt->precision < 20)
			pf_put_longdbl_quick(out, l, fmt);
		else
			pf_ldtoa(out, l, fmt);
	}
	else
	{
		d = va_arg(ap, double);
		if (ft_fabs(d) < (double)ULONG_MAX && fmt->precision < 20)
			pf_putdbl_quick(out, d, fmt);
		else
			pf_dtoa(out, d, fmt);
	}
}
