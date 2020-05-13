/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_double_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/13 02:52:31 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

void		double_conv(char **str, const t_fmt *fmt, va_list ap)
{
	long double		lnb;
	double			nb;
	int				precision;

	if (fmt->is_quad)
		lnb = va_arg(ap, long double);
	else
		nb = va_arg(ap, double);
	if (fmt->has_precision)
		precision = fmt->precision;
	else
		precision = DOUBLE_CONV_DEFAULT_PRECISION;
	if (fmt->is_quad)
		*str = pf_ldtoa(lnb, precision, fmt);
	else
		*str = pf_dtoa(nb, precision, fmt);
}

