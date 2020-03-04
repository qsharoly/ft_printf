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

char		*ft_itoa_base_abs(long long int value, int base, int min_digits, int upcase)
{
	char	*a;
	char	*digits;
	int		n_digits;
	int		pad_size;

	if (min_digits == 0 && value == 0)
	{
		a = malloc(sizeof(*a) * 1);
		a[0] = '\0';
		return (a);
	}
	if (upcase)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";
	n_digits = count_digits(value, base);
	pad_size = min_digits > n_digits ? min_digits - n_digits : 0;
	a = malloc(sizeof(*a) * (n_digits + pad_size + 1));
	do_itoa(a + n_digits + pad_size - 1, value, base, digits);
	a[n_digits + pad_size] = '\0';
	while (pad_size > 0)
	{
		a[pad_size - 1] = '0';
		pad_size--;
	}
	return (a);
}
