/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignum.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:19:11 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/13 07:39:04 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIGNUM_H
# define BIGNUM_H

# define BIG_N_CHUNKS 30
# define CHUNK_TOP_BIT 63
# define CHUNK_N_BITS 64
# define HIGHEST_BIT_MASK (1L << CHUNK_TOP_BIT)
# define CHUNK_MAX_VALUE ULONG_MAX
# define BIG_TO_STR_BUFSIZE (20 * BIG_N_CHUNKS)

typedef unsigned long	t_chunk;

/*
** chunk 0 is the least significant
*/

typedef	struct		s_big
{
	t_chunk	val[BIG_N_CHUNKS];
}					t_big;

typedef struct		s_big_quorem
{
	t_big	quo;
	t_big	rem;
}					t_big_quorem;

t_big	big_zero(void);
t_big	big_shr_one(t_big a);
t_big	big_mul(t_big a, t_big b);
t_big	big_from_chunk(t_chunk small);
t_big	big_raise(t_chunk small_base, t_chunk power);
char	*big_to_string(t_big a);
char	*big_to_string_round(t_big a, int rounding_position);

#endif
