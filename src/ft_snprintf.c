/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2024/07/26 00:54:32 by kith             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

/*
** if no more space left, but we are in the middle of a `conv_foo` function,
** `putc_to_string_buffer` will no-op until the `conv_foo` finishes.
** after that, printing will terminate in `print_args` function.
*/

/*
** if `out->size == 0` we still go through the conversion
** to count how many chars would be written
*/

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
