/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignum.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:19:11 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/24 09:53:00 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIGNUM_H
# define BIGNUM_H

/*
** `t_digit` must be enough to hold (BIG_BASE)
** `t_digit_tmp` must be enough to hold (BIG_BASE * BIG_BASE)
**
*/

# define BIG_N_DIGITS 90
# define BIG_BASE 100000000
# define BIG_DIGIT_CHARS 8
# define BIG_TO_STR_BUFSIZE (BIG_DIGIT_CHARS * BIG_N_DIGITS + 1)

typedef unsigned long	t_digit;
typedef long long		t_digit_tmp;

typedef	struct			s_big
{
	t_digit	val[BIG_N_DIGITS];
	int		used;
}						t_big;

t_big					big_zero(void);
t_big					big_from_number(unsigned long n);
void					add_with_carry_internal(t_big *res, int at_index,
							t_digit_tmp summand);

t_big					big_add(t_big a, t_big b);
t_big					big_mul(t_big a, t_big b);
t_big					big_raise(t_digit n, t_digit power);
char					*big_str(char buf[BIG_TO_STR_BUFSIZE], t_big a);

#endif
