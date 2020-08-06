/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_carry.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:52:19 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:58:53 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_carry(t_big a, int chunk_idx)
{
	while (a.val[chunk_idx] == CHUNK_MAX_VALUE && chunk_idx < BIG_N_CHUNKS - 1)
		chunk_idx++;
	a.val[chunk_idx] += 1;
	return (a);
}
