/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_to_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:10:05 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/23 04:16:54 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "bignum.h"

static char	*copy_digit(char *pos, t_digit value, int fill_at_least)
{
	int	count;

	count = 0;
	while (value)
	{
		*pos = '0' + value % 10;
		value /= 10;
		pos--;
		count++;
	}
	while (count < fill_at_least)
	{
		*pos = '0';
		pos--;
		count++;
	}
	return (pos);
}

char		*big_str(char buf[BIG_TO_STR_BUFSIZE], t_big a)
{
	char	*pos;
	int		i;
	int		used_digits;
	int		fill_at_least;

	pos = &buf[BIG_TO_STR_BUFSIZE - 1];
	*pos = '\0';
	pos--;
	used_digits = BIG_N_DIGITS - 1;
	while (a.val[used_digits] == 0)
	{
		used_digits--;
	}
	used_digits++;
	i = 0;
	while (i < used_digits)
	{
		fill_at_least = i < used_digits - 1 ? BIG_BASE_CHARS : 0;
		pos = copy_digit(pos, a.val[i], fill_at_least);
		i++;
	}
	pos++;
	return (pos);
}
