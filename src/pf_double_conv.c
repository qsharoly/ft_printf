/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_double_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/13 02:13:21 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

void		double_conv(char **str, const t_fmt *fmt, va_list ap)
{
	double	nb;
	int		precision;

	nb = va_arg(ap, double);
	if (fmt->has_precision)
		precision = fmt->precision;
	else
		precision = DOUBLE_CONV_DEFAULT_PRECISION;
	*str = pf_dtoa(nb, precision, fmt);
}
