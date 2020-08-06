/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_sub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:53:13 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 20:01:55 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_sub(t_big a, t_big b)
{
	t_big	diff;
	int		chunk_idx;

	diff = big_zero();
	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		diff.val[chunk_idx] = a.val[chunk_idx] - b.val[chunk_idx];
		if (a.val[chunk_idx] < b.val[chunk_idx])
			b = big_carry(b, chunk_idx + 1);
		chunk_idx++;
	}
	return (diff);
}
