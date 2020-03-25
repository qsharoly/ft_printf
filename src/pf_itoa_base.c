/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:42:01 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/26 17:40:02 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "libftprintf.h"

static int	count_digits(long long int value, int base)
{
	int	count;

	count = 1;
	while (value / base != 0)
	{
		count++;
		value /= base;
	}
	return (count);
}

static int	my_abs(int nbr)
{
	return (nbr > 0 ? nbr : -nbr);
}

static void	do_itoa(char *str, long long int value, int base, const char *digits)
{
	long long int		quo;

	quo = value / base;
	if (quo == 0)
	{
		*str = digits[my_abs(value)];
		return ;
	}
	else
	{
		*str = digits[my_abs(value % base)];
		do_itoa(str - 1, quo, base, digits);
	}
}

char		*pf_itoa_dec(long long int value, int min_digits, char prefix)
{
	char	*buf;
	int		nb_digits;
	int		end;
	int		i;

	nb_digits = count_digits(value, 10);
	if (value == 0)
		nb_digits = 0;
	end = pf_max(min_digits, nb_digits);
	if (prefix)
		end++;
	buf = malloc(sizeof(char) * (end + 1));
	buf[end] = '\0';
	end--;
	do_itoa(buf + end, value, 10, "0123456789");
	i = nb_digits;
	while (i < min_digits)
	{
		buf[end - i] = '0';
		i++;
	}
	if (prefix)
		buf[0] = prefix;
	return (buf);
}
