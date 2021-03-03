/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2021/03/03 11:37:26 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "libftprintf.h"

void			pf_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

static void		print_args(t_stream *out, const char *format, va_list ap)
{
	t_fmt			fmt;

	while (*format)
	{
		if (*format == '%')
		{
			fmt = pf_parse_specifier(format, ap);
			if (fmt.write_arg)
				fmt.write_arg(out, &fmt, ap);
			format += fmt.spec_length;
			continue ;
		}
		pf_putc(*format, out);
		format++;
	}
}

__attribute__((__format__(__printf__, 1, 2)))
int				ft_printf(const char *format, ...)
{
	va_list		ap;
	t_stream	b;
	char		buffer[BUFFER_SIZE];

	b = pf_stream_init(STDOUT_FD, buffer, BUFFER_SIZE, putc_impl_printf);
	init_conv_table();
	va_start(ap, format);
	print_args(&b, format, ap);
	va_end(ap);
	pf_stream_flush(&b);
	return (b.total_written);
}
