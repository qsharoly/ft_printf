/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base_unsigned.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 16:14:34 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/21 16:18:42 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	count_digits(unsigned int value, unsigned int base)
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

static char	make_digit(unsigned int nbr)
{
	return (nbr < 10 ? '0' + nbr : 'a' + nbr - 10);
}

static void	do_itoa(char *str, unsigned int value, unsigned int base)
{
	unsigned int		quo;

	quo = value / base;
	if (quo == 0)
	{
		*str = make_digit(value);
		return ;
	}
	else
	{
		*str = make_digit(value % base);
		do_itoa(str - 1, quo, base);
	}
}

char		*ft_itoa_base_unsigned(unsigned int value, unsigned int base)
{
	char	*a;
	int		n_digits;

	n_digits = count_digits(value, base);
	a = malloc(sizeof(*a) * (n_digits + 1));
	do_itoa(a + n_digits - 1, value, base);
	a[n_digits] = '\0';
	return (a);
}
