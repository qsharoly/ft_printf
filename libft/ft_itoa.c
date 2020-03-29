/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 15:18:53 by qsharoly          #+#    #+#             */
/*   Updated: 2019/09/11 18:11:33 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_itoa(int n)
{
	char	*a;
	char	buf[ITOA_BUF_SIZE];
	int		nb_digits;
	int		is_neg;

	nb_digits = 0;
	is_neg = (n < 0);
	if (n <= 0)
	{
		buf[ITOA_BUF_SIZE - nb_digits++] = -(n % 10) + '0';
		n = -(n / 10);
	}
	while (n > 0)
	{
		buf[ITOA_BUF_SIZE - nb_digits++] = n % 10 + '0';
		n = n / 10;
	}
	if (!(a = malloc(sizeof(*a) * (is_neg + nb_digits + 1))))
			return (NULL);
	if (is_neg)
		a[0] = '-';
	ft_memcpy(a + is_neg, buf + ITOA_BUF_SIZE - nb_digits + 1, nb_digits);
	a[is_neg + nb_digits] = '\0';
	return (a);
}
