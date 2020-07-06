/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 19:43:27 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "libftprintf.h"

/*
** Number and order of g_write_arg and g_get_arg functions
** must correspond to conversion types in enum e_types
*/

static void (*g_write_arg[12])(t_stream *, t_fmt *, union u_pfarg) = {
	conv_percent,
	conv_c,
	conv_s,
	conv_p,
	conv_signed,
	conv_signed,
	conv_unsigned,
	conv_oct,
	conv_hex,
	conv_hex,
	conv_floating,
	conv_default
};

static union u_pfarg (*g_get_arg[12])(va_list, enum e_size) = {
	get_none,
	get_char,
	get_string,
	get_pointer,
	get_signed,
	get_signed,
	get_unsigned,
	get_unsigned,
	get_unsigned,
	get_unsigned,
	get_floating,
	get_none,
};

void		pf_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

void		write_args(t_stream *b, const char *format, va_list ap)
{
	t_fmt			fmt;
	union u_pfarg	arg;

	while (*format)
	{
		if (*format == '%')
		{
			fmt = pf_parse_specifier(format, ap);
			arg = g_get_arg[fmt.type](ap, fmt.size);
			g_write_arg[fmt.type](b, &fmt, arg);
			format += fmt.spec_length;
		}
		else
		{
			pf_putc(*format, b);
			format++;
		}
	}
}

int			ft_printf(const char *format, ...)
{
	va_list		ap;
	t_stream	b;

	pf_stream_init(&b, STDOUT_FD);
	va_start(ap, format);
	write_args(&b, format, ap);
	va_end(ap);
	pf_stream_flush(&b);
	return (b.total_written);
}
