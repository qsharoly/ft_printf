/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2024/07/26 00:33:08 by kith             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "libftprintf.h"

void			pf_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

static void		print_args(t_stream *out, const char *format, va_list *ap)
{
	t_fmt	fmt;
	size_t	spec_length;

	while (*format)
	{
		if (*format == '%')
		{
			spec_length = pf_parse_specifier(&fmt, format, ap);
			if (spec_length > 0)
			{
				write_argument(out, &fmt, ap);
				format += spec_length;
				continue ;
			}
		}
		pf_putc(*format, out);
		format++;
	}
}

static void	putc_to_fd(int c, t_stream *b)
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

__attribute__((__format__(__printf__, 1, 2)))
int				ft_printf(const char *format, ...)
{
	va_list		ap;
	char		buffer[BUFFER_SIZE];
	t_stream	b;

	b = pf_stream_init(STDOUT, buffer, BUFFER_SIZE, putc_to_fd);
	va_start(ap, format);
	print_args(&b, format, &ap);
	va_end(ap);
	pf_stream_flush(&b);
	return (b.total_written);
}

__attribute__((__format__(__printf__, 2, 3)))
int				ft_dprintf(int fd, const char *format, ...)
{
	va_list		ap;
	t_stream	b;
	char		buffer[BUFFER_SIZE];

	b = pf_stream_init(fd, buffer, BUFFER_SIZE, putc_to_fd);
	va_start(ap, format);
	print_args(&b, format, &ap);
	va_end(ap);
	pf_stream_flush(&b);
	return (b.total_written);
}

int				ft_vdprintf(int fd, const char *format, va_list ap)
{
	t_stream	b;
	char		buffer[BUFFER_SIZE];
	va_list		ap_copy;

	b = pf_stream_init(fd, buffer, BUFFER_SIZE, putc_to_fd);
	/* Note: This seemingly unnecessary copy is required in case va_list
	 * is an array type. */
	va_copy(ap_copy, ap);
	print_args(&b, format, &ap_copy);
	va_end(ap_copy);
	pf_stream_flush(&b);
	return (b.total_written);
}
