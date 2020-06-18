/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iabs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 19:40:50 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 19:40:55 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** will overflow on INT_MIN
*/

int		ft_iabs(int i)
{
	return (i < 0 ? -i : i);
}
