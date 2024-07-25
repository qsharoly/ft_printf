/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_stream.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:26:20 by qsharoly          #+#    #+#             */
/*   Updated: 2024/07/26 00:57:32 by kith             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

t_stream	pf_stream_init(int fd, char *buffer, int size,
				enum e_write_mode write_mode,
				void (*custom_putc)(int, t_stream *))
{
	return (t_stream){
		.write_mode = write_mode,
		.putc_custom_callback = custom_putc,
		.data = buffer,
		.size = size,
		.fd = fd,
	};
}

/*
** if data buffer is not empty, write to output
*/

void		pf_stream_flush(t_stream *b)
{
	int		written;

	if (b->used > 0)
	{
		written = write(b->fd, b->data, b->used);
		if (written < 0)
			pf_error("write error\n");
		b->total_written += written;
		b->used = 0;
	}
}

void		pf_putc(int c, t_stream *b)
{
	switch (b->write_mode)
	{
		case WRITE_TO_FD_BUFFERED:
			{
				int		written;

				if (b->used == b->size)
				{
					written = write(b->fd, b->data, b->size);
					if (written < 0)
						pf_error("write error\n");
					b->total_written += written;
					b->used = 0;
				}
				b->data[b->used] = c;
				b->used++;
			}
			break;
		case WRITE_TO_STRING_BUFFER:
			{
				if (b->used == b->size)
					return ;
				b->total_written++;
				if (b->data)
				{
					b->data[b->used] = c;
					b->used++;
				}
			}
			break;
		case CALC_REQUIRED_SIZE:
			{
				b->total_written++;
			}
			break;
		case USE_CUSTOM_PUTC_CALLBACK:
			{
				b->putc_custom_callback(c, b);
			}
			break;
		default:
			;
	}
}
