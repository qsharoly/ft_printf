/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fabs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 09:38:53 by debby             #+#    #+#             */
/*   Updated: 2020/06/13 12:06:43 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "float.h"

double		ft_fabs(double d)
{
	union u_f64	tmp;

	tmp.d = d;
	tmp.bits.sign = 0;
	return (tmp.d);
}

long double	ft_fabsl(long double d)
{
	union u_f80	tmp;

	tmp.d = d;
	tmp.bits.sign = 0;
	return (tmp.d);
}
