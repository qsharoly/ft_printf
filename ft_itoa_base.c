/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:42:01 by qsharoly          #+#    #+#             */
/*   Updated: 2020/01/14 15:52:19 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	count_digits(int value, int base)
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

static char	make_digit(int nbr)
{
	return (nbr < 10 ? '0' + nbr : 'A' + nbr - 10);
}

static void	do_itoa(char *str, int value, int base)
{
	int		quo;

	quo = value / base;
	if (quo == 0)
	{
		*str = make_digit(my_abs(value));
		return ;
	}
	else
	{
		*str = make_digit(my_abs(value % base));
		do_itoa(str - 1, quo, base);
	}
}

char		*ft_itoa_base(int value, int base)
{
	char	*a;
	int		n_digits;

	n_digits = count_digits(value, base);
	if (value < 0 && base == 10)
	{
		a = malloc(sizeof(*a) * (n_digits + 2));
		*a = '-';
		do_itoa(a + n_digits, value, base);
		a[n_digits + 1 ] = '\0';
	}
	else
	{
		a = malloc(sizeof(*a) * (n_digits + 1));
		do_itoa(a + n_digits - 1, value, base);
		a[n_digits] = '\0';
	}
	return (a);
}
