/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 15:09:44 by debby             #+#    #+#             */
/*   Updated: 2020/08/09 12:12:57 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "float.h"

int		ft_isinf64(double d)
{
	union u_f64	tmp;

	tmp.d = d;
	if (tmp.bits.exponent == 0x7ff && tmp.bits.mantissa == 0)
		return (tmp.bits.sign ? -1 : 1);
	else
		return (0);
}

int		ft_isinf(long double d)
{
	union u_f80	tmp;

	tmp.d = d;
	if (tmp.bits.exponent == 0x7fff && tmp.bits.mantissa == 0)
		return (tmp.bits.sign ? -1 : 1);
	else
		return (0);
}
