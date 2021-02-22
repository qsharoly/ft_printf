/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/23 02:33:37 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "libftprintf.h"

/*
** `out->space_left == 0 && out->pos == 0` means we are just counting
** how many chars would be written (`max_size` was specified as `0`)
*/

static void	print_args(t_stream *out, const char *format, va_list ap)
{
	t_fmt			fmt;
	union u_pfarg	arg;

	while ((out->space_left > 0 || (out->space_left == 0 && out->pos == 0))
			&& *format)
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

int			ft_snprintf(char *buffer, int max_size, const char *format, ...)
{
	va_list		ap;
	t_stream	b;

	b = pf_stream_init(STDOUT_FD, buffer, max_size, putc_snprintf_internal);
	va_start(ap, format);
	print_args(&b, format, ap);
	va_end(ap);
	if (buffer && max_size > 0)
	{
		buffer[b.total_written - 1] = '\0';
	}
	return (b.total_written);
}
