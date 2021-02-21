/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_mul.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:40:23 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/21 21:54:29 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_mul(t_big a, t_big b)
{
	t_big		res;
	t_digit		carry;
	t_digit_tmp	prod;
	int			i;
	int			j;

	res = big_zero();
	j = 0;
	carry = 0;
	while (j < BIG_N_DIGITS)
	{
		i = 0;
		while (i < BIG_N_DIGITS)
		{
			if (i + j >= BIG_N_DIGITS)
			{
				break ;
			}
			prod = res.val[i + j] + a.val[i] * b.val[j] + carry;
			res.val[i + j] = prod % BIG_BASE;
			carry = prod / BIG_BASE;
			i++;
		}
		j++;
	}
	return (res);
}
