/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 15:18:53 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/26 02:53:12 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_itoa(int n)
{
	char	*a;
	char	buf[ITOA_BUF_SIZE + 1];
	int		nb_digits;
	int		negative;

	nb_digits = 0;
	negative = (n < 0);
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
	if (!(a = malloc(sizeof(*a) * (negative + nb_digits + 1))))
			return (NULL);
	if (negative)
		a[0] = '-';
	ft_memcpy(a + negative, buf + ITOA_BUF_SIZE - nb_digits + 1, nb_digits);
	a[negative + nb_digits] = '\0';
	return (a);
}
