/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_shl_shr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:38:58 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 20:07:54 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_shl_one(t_big a)
{
	int		i;
	int		prev_top_bit;
	int		top_bit;

	i = 0;
	prev_top_bit = 0;
	while (i < BIG_N_CHUNKS)
	{
		top_bit = a.val[i] >> CHUNK_TOP_BIT;
		a.val[i] <<= 1;
		if (prev_top_bit)
			a.val[i] += 1;
		prev_top_bit = top_bit;
		i++;
	}
	return (a);
}

/*
** big_shl:
** If shift is bigger than total number of bits
** this will return big_zero.
** When local == 0 (shift is proportional to CHUNK_N_BITS),
** a right shift by (CHUNK_N_BITS - local) causes a shift overflow
** and the chunk's value is unchanged (or some other undefined behavior).
** In this case we pretend that the chunk is shifted out entirely
** (becomes 0).
** TODO: protect from negative shift?
*/

t_big	big_shl(t_big a, int shift)
{
	int		global;
	int		local;
	int		i;

	global = shift / CHUNK_N_BITS;
	if (global >= BIG_N_CHUNKS)
		return (big_zero());
	local = shift % CHUNK_N_BITS;
	i = BIG_N_CHUNKS - 1;
	while (i > global)
	{
		a.val[i] = (a.val[i - global] << local)
			+ (local != 0) * (a.val[i - global - 1] >> (CHUNK_N_BITS - local));
		i--;
	}
	if (i >= 0)
	{
		a.val[i] = a.val[0] << local;
		i--;
	}
	while (i >= 0)
	{
		a.val[i] = 0;
		i--;
	}
	return (a);
}

t_big	big_shr_one(t_big a)
{
	int		i;
	int		need_carry;

	i = 0;
	while (i < BIG_N_CHUNKS)
	{
		need_carry = (i < BIG_N_CHUNKS - 1) && (a.val[i + 1] & 1L);
		a.val[i] >>= 1;
		if (need_carry)
			a.val[i] |= HIGHEST_BIT_MASK;
		i++;
	}
	return (a);
}
