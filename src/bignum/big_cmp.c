/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_cmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:37:42 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:37:58 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

int		big_cmp(t_big a, t_big b)
{
	int		chunk_idx;

	chunk_idx = BIG_N_CHUNKS - 1;
	while (chunk_idx >= 0)
	{
		if (a.val[chunk_idx] < b.val[chunk_idx])
			return (-1);
		if (a.val[chunk_idx] > b.val[chunk_idx])
			return (1);
		chunk_idx--;
	}
	return (0);
}
