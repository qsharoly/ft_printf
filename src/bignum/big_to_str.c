/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_to_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:10:05 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/12 22:16:35 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bignum.h"

/*
** caller provides a buffer of size BIG_TO_STR_BUFSIZE
** buffer is filled with digits from right to left
** return value is the pointer to the beginning of the resulting digit sequence
*/

char	*big_str(char buf[BIG_TO_STR_BUFSIZE], t_big a)
{
	t_big_quorem	tmp;
	t_big			radix;
	t_big			zero;
	char			*s;

	tmp.quo = a;
	zero = big_zero();
	radix = big_from_chunk(10);
	s = &buf[BIG_TO_STR_BUFSIZE];
	*s = '\0';
	while (big_cmp(tmp.quo, zero) > 0)
	{
		s--;
		if (s < buf)
		{
			ft_putstr_fd("not enough space for bignum to string conversion\n",
					2);
			return (NULL);
		}
		tmp = big_divmod(tmp.quo, radix);
		*s = '0' + (char)tmp.rem.val[0];
	}
	return (s);
}
