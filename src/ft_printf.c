/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2024/08/15 21:42:07 by kith             ###   ########.fr       */
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

static void	print_args(t_stream *out, const char *format, va_list *ap)
{
	t_fmt	fmt;
	size_t	spec_length;

	while (*format)
	{
		if (out->used >= out->size && out->write_mode != CALC_REQUIRED_SIZE)
			break;
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

__attribute__((__format__(__printf__, 1, 2)))
int				ft_printf(const char *format, ...)
{
	va_list		ap;
	char		buffer[BUFFER_SIZE];
	t_stream	b;

	b = pf_stream_init(STDOUT, buffer, BUFFER_SIZE, WRITE_TO_FD_BUFFERED, NULL);
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

	b = pf_stream_init(fd, buffer, BUFFER_SIZE, WRITE_TO_FD_BUFFERED, NULL);
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

	b = pf_stream_init(fd, buffer, BUFFER_SIZE, WRITE_TO_FD_BUFFERED, NULL);
	/* Note: This seemingly unnecessary copy is required in case va_list
	 * is an array type. */
	va_copy(ap_copy, ap);
	print_args(&b, format, &ap_copy);
	va_end(ap_copy);
	pf_stream_flush(&b);
	return (b.total_written);
}

// https://en.cppreference.com/w/c/io/fprintf
//
// 4) Writes the results to a character string `buffer`.
// At most `bufsz - 1` characters are written. The resulting character string
// will be terminated with a null character, unless `bufsz` is zero.
// If `bufsz` is zero, nothing is written and `buffer` may be a null pointer,
// however the return value (number of bytes that would be written not including
// the null terminator) is still calculated and returned.
__attribute__((__format__(printf, 3, 4)))
int			ft_snprintf(char *buffer, int bufsz, const char *format, ...)
{
	va_list		ap;
	t_stream	b;

	if (bufsz == 0)
		b = pf_stream_init(STDOUT, buffer, bufsz, CALC_REQUIRED_SIZE, NULL);
	else
		b = pf_stream_init(STDOUT, buffer, bufsz, WRITE_TO_STRING_BUFFER, NULL);
	va_start(ap, format);
	print_args(&b, format, &ap);
	va_end(ap);
	if (buffer && bufsz > 0)
	{
		buffer[b.used] = '\0';
	}
	return (b.total_written);
}
