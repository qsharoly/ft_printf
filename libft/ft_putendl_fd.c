/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 17:21:08 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/24 09:27:07 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putendl_fd(int fd, char const *s)
{
	int	len;

	len = ft_strlen(s);
	write(fd, s, len);
	write(fd, "\n", 1);
}
