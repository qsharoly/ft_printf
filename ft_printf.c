/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/17 12:44:08 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include "libftprintf.h"

static int	flag_is_set(const char flags, const char which)
{
	return (flags & which);
}

static int	char_in_str(char needle, const char *hay)
{
	while (*hay)
	{
		if (*hay == needle)
			return (1);
		hay++;
	}
	return (0);
}

/*
** repeated flag symbols are skipped
** if there is no type at end of format, set fmt.type to '\0'
*/

static t_fmt	get_format(const char *str)
{
	t_fmt	fmt;
	char	*ptr;

	ptr = (char *)str + 1;
	fmt.min_field_width = 0;
	fmt.specifier_length = 1;
	fmt.flags = 0;
	while (char_in_str(*ptr, "0-+ #"))
	{
		if (*ptr == '0')
			fmt.flags |= PAD_WITH_ZEROS;
		else if (*ptr == '-')
			fmt.flags |= PAD_FROM_RIGHT;
		else if (*ptr == '+')
			fmt.flags |= PLUS_POSITIVE;
		else if (*ptr == ' ')
			fmt.flags |= SPACE_POSITIVE;
		else if (*ptr == '#')
			fmt.flags |= ALTERNATE_FORM;
		fmt.specifier_length++;
		ptr++;
	}

	if (char_in_str(*ptr, "%sdixX"))
	{
		fmt.type = *ptr;
		fmt.specifier_length++;
	}
	else
		fmt.type = TYPE_MISSING;
	return (fmt);
}

static void make_pad(char *pad, int padlen, char flags)
{
	int		i;
	char	padchar;

	if (flag_is_set(flags, PAD_WITH_ZEROS))
		padchar = '0';
	else
		padchar = ' ';
	pad = malloc(padlen + 1);
	if (pad == NULL)
	{
		write(2, "failed malloc", 13);
		exit(1);
	}
	i = 0;
	while (i < padlen)
	{
		pad[i] = padchar;
		i++;
	}
	pad[padlen] = '\0';
}

static void	put(t_fmt fmt, va_list ap)
{
	char	*str;
	char	*pad;
	char	*prefix;
	int		nb;
	int		padlen;

	if (fmt.type == '%')
	{
		str = "%";
		prefix = "";
	}
	else if (fmt.type == 's')
	{
		str = va_arg(ap, char *);
		prefix = "";
	}
	else if (fmt.type == 'd' || fmt.type == 'i')
	{
		nb = va_arg(ap, int);
		if (nb >= 0)
		{
			if (flag_is_set(fmt.flags, PLUS_POSITIVE))
				prefix = "+";
			else if (flag_is_set(fmt.flags, SPACE_POSITIVE))
				prefix = " ";
			else
				prefix = "";
		}
		else
			prefix = "";
		str = ft_itoa_base(nb, 10);
	}
	else if (fmt.type == 'o')
	{
		str = ft_itoa_base(va_arg(ap, int), 8);
		if (flag_is_set(fmt.flags, ALTERNATE_FORM))
			prefix = "0";
		else
			prefix = "";
	}
	else if (fmt.type == 'x' || fmt.type == 'X')
	{
		str = ft_itoa_base(va_arg(ap, int), 16);
		if (fmt.type == 'X')
			ft_strupper(str);
		if (flag_is_set(fmt.flags, ALTERNATE_FORM))
			prefix = fmt.type == 'x' ? "0x" : "0X";
		else
			prefix = "";
	}
	else
		str = NULL;
	if (str)
	{
		if (ft_strlen(str) + ft_strlen(prefix) < fmt.min_field_width)
		{
			padlen = fmt.min_field_width - ft_strlen(str) - ft_strlen(prefix);
			pad = NULL;
			make_pad(pad, padlen, fmt.flags);
			if (pad)
			{
				write(1, pad, padlen);
				free(pad);
			}
		}
		write(1, prefix, ft_strlen(prefix));
		write(1, str, ft_strlen(str));
	}
	if (fmt.type != 's' && fmt.type != '%')
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
