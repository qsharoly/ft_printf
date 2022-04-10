/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2022/04/10 08:33:17 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

/*
** if no more space left, but we are in the middle of a `conv_foo` function,
** `putc_to_string_buffer` will no-op until the `conv_foo` finishes.
** after that, printing will terminate in `print_args` function.
*/

static void	putc_to_string_buffer(int c, t_stream *b)
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

static void	putc_only_count(int c, t_stream *b)
{
	(void)c;
	b->total_written++;
}

/*
** if `out->size == 0` we still go through the conversion
** to count how many chars would be written
*/

static void	print_args(t_stream *out, const char *format, va_list ap)
{
	t_fmt	fmt;
	size_t	spec_length;

	while (*format)
	{
		if (out->used >= out->size && out->putc != putc_only_count)
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


__attribute__((__format__(printf, 3, 4)))
int			ft_snprintf(char *str, int max, const char *format, ...)
{
	va_list		ap;
	t_stream	b;

	if (max == 0)
		b = pf_stream_init(STDOUT, str, max, putc_only_count);
	else
		b = pf_stream_init(STDOUT, str, max, putc_to_string_buffer);
	va_start(ap, format);
	print_args(&b, format, ap);
	va_end(ap);
	if (str && max > 0)
	{
		str[b.total_written - 1] = '\0';
	}
	return (b.total_written);
}
