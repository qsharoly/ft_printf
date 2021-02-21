/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/21 21:58:00 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "libftprintf.h"

void		pf_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

union u_pfarg	get_arg(va_list ap, const t_fmt *fmt)
{
	union u_pfarg	arg;

	if (fmt->write_arg == conv_percent)
		arg = get_none(ap, fmt->size);
	else if (fmt->write_arg == conv_char)
		arg = get_char(ap, fmt->size);
	else if (fmt->write_arg == conv_str)
		arg = get_string(ap, fmt->size);
	else if (fmt->write_arg == conv_ptr)
		arg = get_pointer(ap, fmt->size);
	else if (fmt->write_arg == conv_signed)
		arg = get_signed(ap, fmt->size);
	else if (fmt->write_arg == conv_unsigned || fmt->write_arg == conv_oct
			|| fmt->write_arg == conv_hex)
		arg = get_unsigned(ap, fmt->size);
	else if (fmt->write_arg == conv_floating)
		arg = get_floating(ap, fmt->size);
	else
		arg = get_none(ap, fmt->size);
	return (arg);
}

static void	print_args(t_stream *out, const char *format, va_list ap)
{
	t_fmt			fmt;
	union u_pfarg	arg;

	while (*format)
	{
		if (*format == '%')
		{
			fmt = pf_specifier_parse(format, ap);
			arg = get_arg(ap, &fmt);
			fmt.write_arg(out, &fmt, arg);
			format += fmt.spec_length;
		}
		else
		{
			pf_putc(*format, out);
			format++;
		}
	}
}

int			ft_printf(const char *format, ...)
{
	va_list		ap;
	t_stream	b;
	char		buffer[BUFFER_SIZE];

	pf_stream_init(&b, STDOUT_FD, buffer, BUFFER_SIZE, putc_printf_internal);
	va_start(ap, format);
	print_args(&b, format, ap);
	va_end(ap);
	pf_stream_flush(&b);
	return (b.total_written);
}
