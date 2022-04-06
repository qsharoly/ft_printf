/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_mul.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:40:23 by qsharoly          #+#    #+#             */
/*   Updated: 2022/04/06 17:16:48 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bignum.h"

void	big_addc_small(t_big *res, int at_index, t_digit_tmp carry)
{
	t_digit_tmp	sum;

	while (carry)
	{
		if (at_index >= BIG_N_DIGITS)
		{
			res->overflow_occured = 1;
			return ;
		}
		if (at_index >= res->used)
		{
			res->used++;
			res->val[at_index] = 0;
		}
		sum = res->val[at_index] + carry;
		res->val[at_index] = sum % BIG_BASE;
		carry = sum / BIG_BASE;
		at_index++;
	}
	if (at_index > res->used)
		res->used = at_index;
}

void	big_mul(t_big *res, const t_big *a, const t_big *b)
{
	t_digit_tmp	tmp;
	int			i;
	int			j;

	big_set_to_small(res, 0);
	j = 0;
	while (j < b->used)
	{
		i = 0;
		while (i < a->used)
		{
			tmp = a->val[i] * b->val[j];
			big_addc_small(res, i + j, tmp);
			if (res->overflow_occured)
				return ;
			i++;
		}
		j++;
	}
}
