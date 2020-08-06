/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_to_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:10:05 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:57:46 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bignum.h"

char	*big_to_string(t_big a)
{
	t_big_quorem	tmp;
	t_big			radix;
	t_big			zero;
	char			buf[BIG_TO_STR_BUFSIZE];
	char			*s;
	int				i;

	tmp.quo = a;
	zero = big_zero();
	radix = big_from_chunk(10);
	i = 0;
	while (big_cmp(tmp.quo, zero) > 0)
	{
		tmp = big_divmod(tmp.quo, radix);
		buf[BIG_TO_STR_BUFSIZE - 1 - i] = '0' + (char)tmp.rem.val[0];
		i++;
	}
	s = malloc(i + 1);
	s[i] = '\0';
	ft_memcpy(s, &buf[BIG_TO_STR_BUFSIZE - i], i);
	return (s);
}

char	*big_to_string_round(t_big a, int rounding_position)
{
	t_big_quorem	tmp;
	t_big			radix;
	t_big			zero;
	char			buf[BIG_TO_STR_BUFSIZE];
	char			*s;
	int				i;

	tmp.quo = a;
	tmp.rem = big_zero();
	zero = big_zero();
	radix = big_from_chunk(10);
	i = 0;
	while (big_cmp(tmp.quo, zero) > 0)
	{
		if (i == rounding_position && tmp.rem.val[0] > 4)
		{
			tmp.quo = big_inc(tmp.quo);
			ft_memset(buf + BIG_TO_STR_BUFSIZE - 1 - i, '0', i + 1);
		}
		tmp = big_divmod(tmp.quo, radix);
		buf[BIG_TO_STR_BUFSIZE - 1 - i] = '0' + (char)tmp.rem.val[0];
		i++;
	}
	if (i == rounding_position && tmp.rem.val[0] > 4)
	{
		tmp.quo = big_inc(tmp.quo);
		ft_memset(buf + BIG_TO_STR_BUFSIZE - 1 - i, '0', i + 1);
		tmp = big_divmod(tmp.quo, radix);
		buf[BIG_TO_STR_BUFSIZE - 1 - i] = '0' + (char)tmp.rem.val[0];
		i++;
	}
	s = malloc(i + 1);
	s[i] = '\0';
	ft_memcpy(s, &buf[BIG_TO_STR_BUFSIZE - i], i);
	return (s);
}
