/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trunc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 12:07:19 by debby             #+#    #+#             */
/*   Updated: 2020/06/18 17:57:19 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "float.h"

/*
** ft_trunc:
** +0.0, -0.0, +Inf, -Inf and NaN will be returned unchanged
*/

double	ft_trunc(double d)
{
	union u_f64		tmp;
	long			e;
	unsigned long	m;

	if (d == 0.0 || ft_isinf(d) || ft_isnan(d))
		return (d);
	if (ft_issub(d))
		return (d > 0.0 ? 0.0 : -0.0);
	tmp.d = d;
	e = tmp.bits.exponent - F64_BIAS;
	if (e < 0)
		return (d > 0.0 ? 0.0 : -0.0);
	if (e > 52)
		return (d);
	m = tmp.bits.mantissa + (1L << 52);
	tmp.bits.mantissa = (m >> (52 - e)) << (52 - e);
	return (tmp.d);
}

long double	ft_truncl(long double d)
{
	union u_f80		tmp;
	long			e;
	unsigned long	m;

	if (d == 0.0 || ft_isinfl(d) || ft_isnanl(d))
		return (d);
	if (ft_issubl(d))
		return (d > 0.0 ? 0.0 : -0.0);
	tmp.d = d;
	e = tmp.bits.exponent - F80_BIAS;
	if (e < 0)
		return (d > 0.0 ? 0.0 : -0.0);
	if (e > 64)
		return (d);
	m = tmp.bits.mantissa;
	tmp.bits.mantissa = (m >> (64 - e)) << (64 - e);
	return (tmp.d);
}
