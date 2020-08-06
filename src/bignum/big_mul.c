/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_mul_div_pow.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:40:23 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:44:56 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_mul(t_big a, t_big b)
{
	t_big	res;
	int		chunk_idx;
	int		j;

	res = big_zero();
	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		j = 0;
		while (j < CHUNK_N_BITS)
		{
			if (b.val[chunk_idx] & (1L << j))
				res = big_add(res, a);
			a = big_shl_one(a);
			j++;
		}
		chunk_idx++;
	}
	return (res);
}
