/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/28 03:31:29 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "libftprintf.h"

/*
** if `out->size == 0` we still go through the conversion
** to count how many chars would be written
** (`putc_snprintf_internal` will do nothing but increase `out->total_written`
** in this case)
*/

static void	print_args(t_stream *out, const char *format, va_list ap)
{
	t_fmt	fmt;

	while ((out->space_left > 0 || (out->size == 0)) && *format)
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

int			ft_snprintf(char *buffer, int max_size, const char *format, ...)
{
	va_list		ap;
	t_stream	b;

	b = pf_stream_init(STDOUT_FD, buffer, max_size, putc_snprintf_internal);
	init_conv_table();
	va_start(ap, format);
	print_args(&b, format, ap);
	va_end(ap);
	if (buffer && max_size > 0)
	{
		buffer[b.total_written - 1] = '\0';
	}
	return (b.total_written);
}
