/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_issub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 15:11:29 by debby             #+#    #+#             */
/*   Updated: 2020/06/18 18:00:58 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "float.h"

int		ft_issub(double d)
{
	union u_f64	tmp;

	tmp.d = d;
	if (tmp.bits.exponent == 0)
		return (1);
	else
		return (0);
}

int		ft_issubl(long double d)
{
	union u_f80	tmp;

	tmp.d = d;
	if (tmp.bits.exponent == 0)
		return (1);
	else
		return (0);
}
