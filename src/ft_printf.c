/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/17 08:24:19 by qsharoly         ###   ########.fr       */
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

void	write_args(t_buffer *b, const char *format, va_list ap)
{
	t_fmt		fmt;
	char		*cur;

	cur = (char *)format;
	while (*cur)
	{
		if (*cur == '%')
		{
			fmt = pf_parse_specifier(cur);
			fmt.write_arg(b, &fmt, ap);
			cur += fmt.spec_length;
			format = cur;
		}
		else
		{
			pf_putc(*cur, b);
			cur++;
		}
	}
}

void			pf_buffer_init(t_buffer *b, int target_fd)
{
	b->target_fd = target_fd;
	b->total_written = 0;
	b->space_left = BUFFER_SIZE;
	b->pos = 0;
}

/*
** if buffer is not empty, write it's data to output
*/
void			pf_buffer_flush(t_buffer *b)
{
	if (b->space_left != BUFFER_SIZE)
	{
		b->total_written += write(b->target_fd, b->data, BUFFER_SIZE - b->space_left);
		b->pos = 0;
		b->space_left = BUFFER_SIZE;
	}
}

void			pf_putc(int c, t_buffer *b)
{
	int		written;

	if (b->space_left == 0)
	{
		written = write(b->target_fd, b->data, BUFFER_SIZE);
		if (written < 0)
			pf_error("write error\n");
		b->total_written += written;
		b->pos = 0;
		b->space_left = BUFFER_SIZE;
	}
	b->data[b->pos] = c;
	b->pos++;
	b->space_left--;
}

void			pf_puts(char *s, t_buffer *b)
{
	while (*s)
	{
		pf_putc(*s, b);
		s++;
	}
}

void			pf_nputs(char *s, int len, t_buffer *b)
{
	while (*s && len)
	{
		pf_putc(*s, b);
		s++;
		len--;
	}
}


#define STDOUT_FD 1
int				ft_printf(const char *format, ...)
{
	va_list		ap;
	t_buffer	b;

	pf_buffer_init(&b, STDOUT_FD);
	va_start(ap, format);
	write_args(&b, format, ap);
	va_end(ap);
	pf_buffer_flush(&b);
	return (b.total_written);
}
