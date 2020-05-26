/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_double_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/26 02:46:20 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

void		double_conv(t_buffer *out, const t_fmt *fmt, va_list ap)
{
	/*
	long double		lnb;
	*/
	double			nb;
	int				precision;

	/*
	if (fmt->is_quad)
		lnb = va_arg(ap, long double);
	else
	*/
		nb = va_arg(ap, double);
	if (fmt->has_precision)
		precision = fmt->precision;
	else
		precision = DTOA_DEFAULT_PRECISION;
	/*
	if (fmt->is_quad)
		pf_ldtoa(out, lnb, precision, fmt);
	else
	*/
		pf_dtoa(out, nb, precision, fmt);
}
