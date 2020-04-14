/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_utoa_dec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 16:14:34 by qsharoly          #+#    #+#             */
/*   Updated: 2020/04/14 13:28:05 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	count_digits(unsigned long long int value, unsigned int base)
{
	int		count;

	count = 1;
	while (value / base != 0)
	{
		count++;
		value /= base;
	}
	return (count);
}

static void	do_itoa(char *str, unsigned long long value,
				unsigned base, const char *digits)
{
	unsigned long long	quo;

	quo = value / base;
	if (quo == 0)
	{
		*str = digits[value];
		return ;
	}
	else
	{
		*str = digits[value % base];
		do_itoa(str - 1, quo, base, digits);
	}
}

char		*pf_utoa_base(unsigned long long value, unsigned base,
				int min_digits, int upcase)
{
	char	*a;
	char	*digits;
	int		n;
	int		pad_size;

	if (min_digits == 0 && value == 0)
	{
		a = malloc(sizeof(*a) * 1);
		a[0] = '\0';
		return (a);
	}
	digits = upcase ? "0123456789ABCDEF" : "0123456789abcdef";
	n = count_digits(value, base);
	pad_size = min_digits > n ? min_digits - n : 0;
	a = malloc(sizeof(*a) * (pad_size + n + 1));
	do_itoa(a + pad_size + n - 1, value, base, digits);
	a[pad_size + n] = '\0';
	while (pad_size > 0)
	{
		a[pad_size - 1] = '0';
		pad_size--;
	}
	return (a);
}
