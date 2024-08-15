/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_stream.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:26:20 by qsharoly          #+#    #+#             */
/*   Updated: 2024/08/15 22:22:46 by kith             ###   ########.fr       */
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
		.custom_putc_callback = custom_putc,
		.data = buffer,
		.size = size,
		.fd = fd,
	};
}

void		pf_stream_flush(t_stream *out)
{
	if (out->write_mode == WRITE_TO_FD_BUFFERED && out->used > 0)
	{
		int written = write(out->fd, out->data, out->used);
		if (written < 0)
			pf_error("write error\n");
		out->total_written += written;
		out->used = 0;
	}
}

void		pf_putc(int c, t_stream *out)
{
	switch (out->write_mode)
	{
		case WRITE_TO_FD_BUFFERED:
			{
				if (out->used == out->size)
				{
					int written = write(out->fd, out->data, out->size);
					if (written < 0)
						pf_error("write error\n");
					out->total_written += written;
					out->used = 0;
				}
				out->data[out->used] = c;
				out->used++;
			}
			break;
		case WRITE_TO_STRING_BUFFER:
			{
				if (out->used == out->size)
					return ;
				out->total_written++;
				out->data[out->used] = c;
				out->used++;
			}
			break;
		case CALC_REQUIRED_SIZE: { out->total_written++; } break;
		case USE_CUSTOM_PUTC_CALLBACK: { out->custom_putc_callback(c, out); } break;
		default:
			;
	}
}
