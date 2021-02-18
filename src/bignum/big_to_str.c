/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_to_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:10:05 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/18 19:10:41 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "bignum.h"

/*
** caller provides a buffer of size BIG_TO_STR_BUFSIZE
** buffer is filled with digits from right to left
** return value is the pointer to the beginning of the resulting digit sequence
*/

char	*big_str(char buf[BIG_TO_STR_BUFSIZE], t_big a)
{
	char	*s;
	int		chars;
	int		i;
	int		used_digits;
	t_digit	value;

	s = &buf[BIG_TO_STR_BUFSIZE - 1];
	*s = '\0';
	s--;
	used_digits = BIG_N_DIGITS - 1;
	while (a.val[used_digits] == 0) {
		used_digits--;
	}
	used_digits += 1;
	i = 0;
	while (i < used_digits)
	{
		chars = 0;
		value = a.val[i];
		while (value)
		{
			*s = '0' + value % 10;
			value /= 10;
			s--;
			chars++;
		}
		if (i == used_digits - 1)
		{
			break;
		}
		while (chars < BIG_BASE_CHARS)
		{
			*s = '0';
			s--;
			chars++;
		}
		i++;
	}
	s++;
	return (s);
}
