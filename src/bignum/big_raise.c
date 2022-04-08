/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_raise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:45:05 by qsharoly          #+#    #+#             */
/*   Updated: 2022/04/08 06:06:31 by debby            ###   ########.fr       */
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

void	big_mul_by_small(t_big *res, t_digit b)
{
	int carry = 0;
	int i = 0;
	while (i < res->used)
	{
		t_digit_tmp prod = res->val[i] * b + carry;
		res->val[i] = prod % BIG_BASE;
		carry = prod / BIG_BASE;
		i++;
	}
	big_addc_small(res, i, carry);
}

void	big_raise(t_big *res, t_digit base, t_digit power)
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
	big_set_to_small(res, 1);
	t_digit i = 0;
	while (i < power)
	{
		big_mul_by_small(res, base);
		i++;
	}
}
