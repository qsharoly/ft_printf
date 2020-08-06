/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_divmod.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:44:00 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 20:06:13 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

static int		big_top_bit_idx(t_big a)
{
	int		chunk_idx;
	t_chunk	top_chunk;
	int		i;

	chunk_idx = BIG_N_CHUNKS - 1;
	while (a.val[chunk_idx] == 0)
		chunk_idx--;
	i = chunk_idx * CHUNK_N_BITS;
	top_chunk = a.val[chunk_idx];
	while (top_chunk)
	{
		top_chunk >>= 1;
		i++;
	}
	return (i);
}

t_big_quorem	big_divmod(t_big top, t_big bot)
{
	t_big_quorem	out;
	int				shift;

	if (big_cmp(top, bot) < 0)
	{
		out.quo = big_zero();
		out.rem = top;
		return (out);
	}
	shift = big_top_bit_idx(top) - big_top_bit_idx(bot);
	bot = big_shl(bot, shift);
	out.quo = big_zero();
	while (shift >= 0)
	{
		if (big_cmp(top, bot) >= 0)
		{
			top = big_sub(top, bot);
			out.quo.val[shift / CHUNK_N_BITS] |= (1L << (shift % CHUNK_N_BITS));
		}
		bot = big_shr_one(bot);
		shift--;
	}
	out.rem = top;
	return (out);
}
