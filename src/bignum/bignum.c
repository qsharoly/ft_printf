/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignum.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:10:05 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/13 11:58:32 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "limits.h"
#include "libft.h"
#include "bignum.h"

t_big	big_zero(void)
{
	t_big		a;

	ft_bzero(&a, sizeof(a));
	return (a);
}

t_big	big_carry(t_big a, int chunk_idx)
{
	while (a.val[chunk_idx] == CHUNK_MAX_VALUE && chunk_idx < BIG_N_CHUNKS - 1)
		chunk_idx++;
	a.val[chunk_idx] += 1;
	return (a);
}

t_big	big_inc(t_big a)
{
	a = big_carry(a, 0);
	return (a);
}

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

t_big	big_shl_one(t_big a)
{
	int		chunk_idx;
	int		prev_top_bit;
	int		top_bit;

	chunk_idx = 0;
	prev_top_bit = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		top_bit = a.val[chunk_idx] >> CHUNK_TOP_BIT;
		a.val[chunk_idx] <<= 1;
		if (prev_top_bit)
			a.val[chunk_idx] += 1;
		prev_top_bit = top_bit;
		chunk_idx++;
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
	int		chunk_idx;

	global = shift / CHUNK_N_BITS;
	if (global >= BIG_N_CHUNKS)
		return (big_zero());
	local = shift % CHUNK_N_BITS;
	chunk_idx = BIG_N_CHUNKS - 1;
	while (chunk_idx > global)
	{
		a.val[chunk_idx] = (a.val[chunk_idx - global] << local)
			+ (local != 0) * (a.val[chunk_idx - global - 1] >> (CHUNK_N_BITS - local));
		chunk_idx--;
	}
	if (chunk_idx >= 0)
	{
		a.val[chunk_idx] = a.val[0] << local;
		chunk_idx--;
	}
	while (chunk_idx >= 0)
	{
		a.val[chunk_idx] = 0;
		chunk_idx--;
	}
	return (a);
}

t_big	big_shr_one(t_big a)
{
	int		chunk_idx;
	int		need_carry;

	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		need_carry = (chunk_idx < BIG_N_CHUNKS - 1) && (a.val[chunk_idx + 1] & 1L);
		a.val[chunk_idx] >>= 1;
		if (need_carry)
			a.val[chunk_idx] |= HIGHEST_BIT_MASK;
		chunk_idx++;
	}
	return (a);
}

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

t_big	big_from_chunk(t_chunk small)
{
	t_big	big;

	big = big_zero();
	big.val[0] = small;
	return (big);
}

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

int		big_top_bit(t_big a)
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
	int			shift;
	//int			i;

	if (big_cmp(top, bot) < 0)
	{
		out.quo = big_zero();
		out.rem = top;
		return (out);
	}
	shift = big_top_bit(top) - big_top_bit(bot);
	bot = big_shl(bot, shift);
	/*i = 0;
	while (i < shift)
	{
		bot = big_shl_one(bot);
		i++;
	}
	*/
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

char	*big_to_string(t_big a)
{
	t_big_quorem	tmp;
	t_big			radix;
	t_big			zero;
	char			buf[BIG_TO_STR_BUFSIZE];
	char			*s;
	int				i;

#if 1
	ft_memset(buf, 'q', BIG_TO_STR_BUFSIZE);
#endif
	tmp.quo = a;
	zero = big_zero();
	radix = big_from_chunk(10);
	i = 0;
	while (big_cmp(tmp.quo, zero) > 0)
	{
		tmp = big_divmod(tmp.quo, radix);
		buf[BIG_TO_STR_BUFSIZE - 1 - i] = '0' + (char)tmp.rem.val[0];
		i++;
	}
	s = malloc(i + 1);
	s[i] = '\0';
	ft_memcpy(s, &buf[BIG_TO_STR_BUFSIZE - i], i);
	return (s);
}

char	*big_to_string_round(t_big a, int rounding_position)
{
	t_big_quorem	tmp;
	t_big			radix;
	t_big			zero;
	char			buf[BIG_TO_STR_BUFSIZE];
	char			*s;
	int				i;

#if 1
	ft_memset(buf, 'q', BIG_TO_STR_BUFSIZE);
#endif
	tmp.quo = a;
	tmp.rem = big_zero();
	zero = big_zero();
	radix = big_from_chunk(10);
	i = 0;
	while (big_cmp(tmp.quo, zero) > 0)
	{
		if (i == rounding_position && tmp.rem.val[0] > 4)
		{
			tmp.quo = big_inc(tmp.quo);
			ft_memset(buf + BIG_TO_STR_BUFSIZE - 1 - i, '0', i + 1);
		}
		tmp = big_divmod(tmp.quo, radix);
		buf[BIG_TO_STR_BUFSIZE - 1 - i] = '0' + (char)tmp.rem.val[0];
		i++;
	}
	if (i == rounding_position && tmp.rem.val[0] > 4)
	{
		tmp.quo = big_inc(tmp.quo);
		ft_memset(buf + BIG_TO_STR_BUFSIZE - 1 - i, '0', i + 1);
		tmp = big_divmod(tmp.quo, radix);
		buf[BIG_TO_STR_BUFSIZE - 1 - i] = '0' + (char)tmp.rem.val[0];
		i++;
	}
	s = malloc(i + 1);
	s[i] = '\0';
	ft_memcpy(s, &buf[BIG_TO_STR_BUFSIZE - i], i);
	return (s);
}

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
