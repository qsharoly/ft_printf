/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:42:01 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/26 15:22:54 by qsharoly         ###   ########.fr       */
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
	return (nbr < 10 ? '0' + nbr : 'a' + nbr - 10);
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

char		*ft_itoa_base_abs(int value, int base, int min_digits)
{
	char	*a;
	int		n_digits;
	int		pad_size;

	n_digits = count_digits(value, base);
	pad_size = min_digits > n_digits ? min_digits - n_digits : 0;
	a = malloc(sizeof(*a) * (n_digits + pad_size + 1));
	do_itoa(a + n_digits + pad_size - 1, value, base);
	a[n_digits + pad_size] = '\0';
	while (pad_size > 0)
	{
		a[pad_size - 1] = '0';
		pad_size--;
	}
	return (a);
}
