/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:35:29 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/18 16:16:18 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

t_big	big_add(t_big a, t_big b)
{
	t_big		res;
	t_digit		carry;
	t_digit_tmp	sum;
	int			i;

	res = big_zero();
	i = 0;
	carry = 0;
	while (i < BIG_N_DIGITS)
	{
		sum = a.val[i] + b.val[i] + carry;
		res.val[i] = sum % BIG_BASE;
		carry = sum / BIG_BASE;
		i++;
	}
	return (res);
}
