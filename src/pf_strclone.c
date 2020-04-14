/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_strclone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 13:26:51 by qsharoly          #+#    #+#             */
/*   Updated: 2020/04/14 13:27:05 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

char	*pf_strclone(const char *src)
{
	char	*clone;

	clone = ft_strdup(src);
	if (!clone)
		pf_error("malloc error");
	return (clone);
}
