/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fabs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 09:38:53 by debby             #+#    #+#             */
/*   Updated: 2020/08/09 12:11:52 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "float.h"

double		ft_fabs64(double d)
{
	union u_f64	tmp;

	tmp.d = d;
	tmp.bits.sign = 0;
	return (tmp.d);
}

long double	ft_fabs(long double d)
{
	union u_f80	tmp;

	tmp.d = d;
	tmp.bits.sign = 0;
	return (tmp.d);
}
