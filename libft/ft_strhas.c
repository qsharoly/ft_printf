/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strhas.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 00:56:49 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/18 00:59:54 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** won't match on c == '\0'
** (in contrast to ft_strchr())
*/

char	*ft_strhas(char const *s, int c)
{
	return (ft_memchr(s, c, ft_strlen(s)));
}
