/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_raise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:45:05 by qsharoly          #+#    #+#             */
/*   Updated: 2022/04/06 16:45:55 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

void	big_set_to_small(t_big *a, t_digit n)
{
	if (n > BIG_BASE)
	{
		a->used = 2;
		a->val[0] = n % BIG_BASE;
		a->val[1] = n / BIG_BASE;
	}
	else
	{
		a->used = 1;
		a->val[0] = n;
	}
}

void	big_shallow_swap(t_big *a, t_big *b)
{
	t_big	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	big_raise(t_big *res, t_big *tmp1, t_big *tmp2, t_digit base, t_digit power)
{
	if (base == 0 && power == 0)
	{
		big_set_to_small(res, 1);
		return;
	}
	if (base == 0)
	{
		big_set_to_small(res, 0);
		return;
	}
	big_set_to_small(tmp1, 1);
	big_set_to_small(tmp2, base);
	t_digit i = 0;
	while (i < power)
	{
		big_mul(res, tmp1, tmp2);
		big_shallow_swap(res, tmp1);
		i++;
		if (i < power)
		{
			big_mul(res, tmp1, tmp2);
			big_shallow_swap(res, tmp1);
			i++;
		}
	}
	big_shallow_swap(res, tmp1);
}
