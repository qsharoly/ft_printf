/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/01/14 16:16:46 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include "libftprintf.h"

static t_fmt	get_format(const char * str)
{
	t_fmt	f;

	str++;
	if (*str == 's')
		f.type = 's';
	else if (*str == 'd')
		f.type = 'd';
	else if (*str == '%')
		f.type = '%';
	f.specifier_length = 2;
	return (f);
}

static void	put(t_fmt fmt, va_list ap)
{
	char	*str;

	if (fmt.type == 's')
		str = va_arg(ap, char *);
	else if (fmt.type == 'd')
		str = ft_itoa_base(va_arg(ap, int), 10);
	else if (fmt.type == '%')
		str = "%";
	else
		str = NULL;
	write(1, str, ft_strlen(str));
	if (fmt.type == 'd')
		free(str);
}

int				ft_printf(const char * format, ...)
{
	char	*start;
	char	*cur;
	va_list	ap;
	t_fmt	f;

	start = (char *)format;
	cur = (char *)format;
	va_start(ap, format);
	while (*cur)
	{
		if (*cur == '%')
		{
			write(1, start, cur - start);
			f = get_format(cur);
			put(f, ap);
			start = cur + f.specifier_length;
			cur = start;
		}
		else
			cur++;
	}
	write(1, start, cur - start);
	va_end(ap);
	return (0);
}

int		main(void)
{
	ft_printf("abc%def\n", 5);
	ft_printf("abc%%ef\n");
	ft_printf("abc%sef\n", "qwerty");
	ft_printf("my %s equals %d%%\n", "value", -123);
	return (0);
}
