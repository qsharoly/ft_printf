/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnan.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 15:10:55 by debby             #+#    #+#             */
/*   Updated: 2020/08/09 12:10:42 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "float.h"

int		ft_isnan64(double d)
{
	union u_f64	tmp;

	tmp.d = d;
	if (tmp.bits.exponent == 0x7ff && tmp.bits.mantissa != 0)
		return (tmp.bits.sign ? -1 : 1);
	else
		return (0);
}

int		ft_isnan(long double d)
{
	union u_f80	tmp;

	tmp.d = d;
	if (tmp.bits.exponent == 0x7fff && tmp.bits.mantissa != 0)
		return (tmp.bits.sign ? -1 : 1);
	else
		return (0);
}
