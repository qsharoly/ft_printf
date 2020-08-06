/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_pow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:45:05 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:45:12 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_raise(t_chunk small_base, t_chunk power)
{
	t_big	res;
	t_big	base;

	if (small_base == 0 && power == 0)
		return (big_from_chunk(1));
	if (small_base == 0)
		return (big_from_chunk(0));
	res = big_from_chunk(1);
	base = big_from_chunk(small_base);
	while (power-- > 0)
		res = big_mul(res, base);
	return (res);
}
