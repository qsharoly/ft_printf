/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_stream.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:26:20 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/23 02:34:11 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

t_stream	pf_stream_init(int fd, char *buffer, int size,
				void (*putc)(int, t_stream *))
{
	t_stream b;

	b.putc = putc;
	b.data = buffer;
	b.size = size;
	b.fd = fd;
	b.total_written = 0;
	b.space_left = size;
	b.pos = 0;
	return (b);
}

/*
** if data buffer is not empty, write to output
*/

void		pf_stream_flush(t_stream *b)
{
	int		written;

	if (b->space_left != b->size)
	{
		written = write(b->fd, b->data, b->size - b->space_left);
		if (written < 0)
			pf_error("write error\n");
		b->total_written += written;
		b->pos = 0;
		b->space_left = b->size;
	}
}

void		pf_putc(int c, t_stream *out)
{
	out->putc(c, out);
}

void		putc_printf_internal(int c, t_stream *b)
{
	int		written;

	if (b->space_left == 0)
	{
		written = write(b->fd, b->data, b->size);
		if (written < 0)
			pf_error("write error\n");
		b->total_written += written;
		b->pos = 0;
		b->space_left = b->size;
	}
	b->data[b->pos] = c;
	b->pos++;
	b->space_left--;
}

/*
** if no more space left, but we are in the middle of a variable,
** `putc_snprintf_internal` will do a no-op until the variable finishes.
** after that, printing will terminate in `print_args` function.
**
** `total_written` is always incremented to count how many characters
** would be written when `max_size` is set to 0.
*/

void		putc_snprintf_internal(int c, t_stream *b)
{
	b->total_written++;
	if (b->space_left == 0)
		return ;
	if (b->data)
	{
		b->data[b->pos] = c;
		b->pos++;
		b->space_left--;
	}
}
