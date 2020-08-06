/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_stream.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:26:20 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:26:25 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

void		pf_stream_init(t_stream *b, int target_fd)
{
	b->fd = target_fd;
	b->total_written = 0;
	b->space_left = BUFFER_SIZE;
	b->pos = 0;
}

/*
** if data buffer is not empty, write to output
*/

void		pf_stream_flush(t_stream *b)
{
	int		written;

	if (b->space_left != BUFFER_SIZE)
	{
		written = write(b->fd, b->data, BUFFER_SIZE - b->space_left);
		if (written < 0)
			pf_error("write error\n");
		b->total_written += written;
		b->pos = 0;
		b->space_left = BUFFER_SIZE;
	}
}

void		pf_putc(int c, t_stream *b)
{
	int		written;

	if (b->space_left == 0)
	{
		written = write(b->fd, b->data, BUFFER_SIZE);
		if (written < 0)
			pf_error("write error\n");
		b->total_written += written;
		b->pos = 0;
		b->space_left = BUFFER_SIZE;
	}
	b->data[b->pos] = c;
	b->pos++;
	b->space_left--;
}

void		pf_puts(const char *s, t_stream *b)
{
	while (*s)
	{
		pf_putc(*s, b);
		s++;
	}
}

void		pf_nputs(const char *s, int len, t_stream *b)
{
	while (*s && len-- > 0)
	{
		pf_putc(*s, b);
		s++;
	}
}
