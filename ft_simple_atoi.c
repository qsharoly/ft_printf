/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simple_atoi.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 13:15:46 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/17 13:21:11 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static char	*skip_digits(char *p)
{
	while (*p >= '0' && *p <= '9')
		p++;
	return (p);
}

int			ft_simple_atoi(const char *str)
{
	int		nbr;
	char	*start;
	char	*end;

	start = (char *)str;
	end = skip_digits(start);
	nbr = 0;
	while (start < end)
	{
		nbr = nbr * 10 + (*start - '0');
		start++;
	}
	return (nbr);
}
