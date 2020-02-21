/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/21 17:36:44 by qsharoly         ###   ########.fr       */
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
*/

static t_fmt	get_format(const char *str)
{
	t_fmt	fmt;
	char	*ptr;

	ptr = (char *)str + 1;
	fmt.min_field_width = 0;
	fmt.specifier_length = 1;
	fmt.precision = -1;
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
	fmt.min_field_width = ft_simple_atoi(ptr);
	while (*ptr >= '0' && *ptr <= '9')
	{
		ptr++;
		fmt.specifier_length++;
	}
	if (*ptr == '.')
	{
		ptr++;
		fmt.specifier_length++;
		fmt.precision = ft_simple_atoi(ptr);
		while (*ptr >= '0' && *ptr <= '9')
		{
			ptr++;
			fmt.specifier_length++;
		}
	}
	if (char_in_str(*ptr, "%sdioxX"))
	{
		fmt.type = *ptr;
		fmt.specifier_length++;
	}
	else
		fmt.type = TYPE_MISSING;
	return (fmt);
}

static char *make_pad(int padlen, char flags)
{
	int		i;
	char	*pad;
	char	padchar;

	if (flag_is_set(flags, PAD_WITH_ZEROS) && !flag_is_set(flags, PAD_FROM_RIGHT))
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
	return (pad);
}

static void	put(t_fmt fmt, va_list ap, int *total)
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
		if (str == NULL)
			str = "(null)";
		prefix = "";
	}
	else if (fmt.type == 'd' || fmt.type == 'i')
	{
		nb = va_arg(ap, int);
		if (fmt.precision == 0)
			str = "";
		else
			str = ft_itoa_base_abs(nb, 10);
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
			prefix = "-";
	}
	else if (fmt.type == 'o')
	{
		nb = va_arg(ap, int);
		if (fmt.precision == 0)
			str = "";
		else
			str = ft_itoa_base_unsigned((unsigned int)nb, 8);
		if (flag_is_set(fmt.flags, ALTERNATE_FORM) && nb != 0)
			prefix = "0";
		else
			prefix = "";
	}
	else if (fmt.type == 'x' || fmt.type == 'X')
	{
		nb = va_arg(ap, int);
		if (fmt.precision == 0)
			str = "";
		else
			str = ft_itoa_base_unsigned((unsigned int)nb, 16);
		if (fmt.type == 'X')
			ft_strupper(str);
		if (flag_is_set(fmt.flags, ALTERNATE_FORM) && nb != 0)
			prefix = fmt.type == 'x' ? "0x" : "0X";
		else
			prefix = "";
	}
	else
	{
		str = NULL;
		prefix = "";
	}
	if (str)
	{
		*total += ft_strlen(str) + ft_strlen(prefix);
		padlen = fmt.min_field_width - ft_strlen(str) - ft_strlen(prefix);
		pad = NULL;
		if (padlen > 0)
		{
			pad = make_pad(padlen, fmt.flags);
			*total += padlen;
		}
		if (pad && !flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		{
			if (flag_is_set(fmt.flags, PAD_WITH_ZEROS) && (fmt.type == 'd' || fmt.type == 'i' || flag_is_set(fmt.flags, ALTERNATE_FORM)))
			{
				write(1, prefix, ft_strlen(prefix));
				write(1, pad, padlen);
			}
			else
			{
				write(1, pad, padlen);
				write(1, prefix, ft_strlen(prefix));
			}
			write(1, str, ft_strlen(str));
		}
		else if (pad && flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		{
			write(1, prefix, ft_strlen(prefix));
			write(1, str, ft_strlen(str));
			write(1, pad, padlen);
		}
		else
		{
			write(1, prefix, ft_strlen(prefix));
			write(1, str, ft_strlen(str));
		}
		free(pad);
	}
	if (fmt.type != 's' && fmt.type != '%' && fmt.precision != 0)
		free(str);
}

int				ft_printf(const char * format, ...)
{
	char	*start;
	char	*cur;
	va_list	ap;
	t_fmt	f;
	int		total;

	total = 0;
	start = (char *)format;
	cur = (char *)format;
	va_start(ap, format);
	while (*cur)
	{
		if (*cur == '%')
		{
			write(1, start, cur - start);
			total += cur - start;
			f = get_format(cur);
			put(f, ap, &total);
			start = cur + f.specifier_length;
			cur = start;
		}
		else
			cur++;
	}
	write(1, start, cur - start);
	total += cur - start;
	va_end(ap);
	return (total);
}
