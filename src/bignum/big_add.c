/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:35:29 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:53:05 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_add(t_big a, t_big b)
{
	t_big	sum;
	int		chunk_idx;

	sum = big_zero();
	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		sum.val[chunk_idx] = a.val[chunk_idx] + b.val[chunk_idx];
		if (a.val[chunk_idx] > CHUNK_MAX_VALUE - b.val[chunk_idx])
			a = big_carry(a, chunk_idx + 1);
		chunk_idx++;
	}
	return (sum);
}
