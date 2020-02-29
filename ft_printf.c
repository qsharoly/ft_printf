/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/28 20:53:49 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static void (*g_integer_to_string[4][5])(char **, char **, t_fmt, va_list) = {
	{hhd, hd, d, ld, lld},
	{hhu, hu, u, lu, llu},
	{hho, ho, o, lo, llo},
	{hhx, hx, x, lx, llx}
};

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

static void	(*choose_to_string_func(int size, char type))(char **, char **, t_fmt, va_list)
{
	if (type == '%')
		return (modulo_to_string);
	else if (type == 'c')
		return (c_to_string);
	else if (type == 's')
		return (s_to_string);
	else if (type == 'p')
		return (p_to_string);
	else if (type == 'd' || type == 'i')
		return (g_integer_to_string[SIGNED][size]);
	else if (type == 'u')
		return (g_integer_to_string[UNSIGNED][size]);
	else if (type == 'o')
		return (g_integer_to_string[OCTAL][size]);
	else if (type == 'x' || type == 'X')
		return (g_integer_to_string[HEX][size]);
	else
		return (default_to_string);
}

/*
** repeated flag symbols are skipped
*/

static t_fmt	parse_format(const char *str)
{
	t_fmt	fmt;
	char	*ptr;
	int		size;

	ptr = (char *)str + 1;
	fmt.min_field_width = 0;
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
		ptr++;
	}
	fmt.min_field_width = ft_simple_atoi(ptr);
	while (*ptr >= '0' && *ptr <= '9')
		ptr++;
	if (*ptr == '.')
	{
		ptr++;
		fmt.precision = ft_simple_atoi(ptr);
		while (*ptr >= '0' && *ptr <= '9')
			ptr++;
	}
	if (*ptr == 'h' && *(ptr + 1) == 'h')
	{
		ptr += 2;
		size = HH;
	}
	else if (*ptr == 'h')
	{
		ptr++;
		size = H;
	}
	else if (*ptr == 'l' && *(ptr + 1) == 'l')
	{
		ptr += 2;
		size = LL;
	}
	else if (*ptr == 'l')
	{
		ptr++;
		size = L;
	}
	else
		size = REGULAR;
	if (char_in_str(*ptr, "%cspdiuoxX"))
	{
		fmt.type = *ptr;
		ptr++;
	}
	else
		fmt.type = TYPE_MISSING;
	fmt.to_string = choose_to_string_func(size, fmt.type);
	fmt.specifier_length = ptr - str;
	return (fmt);
}

static char *make_pad(int padlen, t_fmt fmt)
{
	int		i;
	char	*pad;
	char	padchar;

	if (flag_is_set(fmt.flags, PAD_WITH_ZEROS) && !flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		padchar = '0';
	else
		padchar = ' ';
	if (flag_is_set(fmt.flags, PAD_WITH_ZEROS) && (fmt.type == 'd' || fmt.type == 'i') && fmt.precision != -1)
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
	int		padlen;
	int		strlen;

	str = NULL;
	prefix = NULL;
	fmt.to_string(&str, &prefix, fmt, ap);
	if (str != NULL)
	{
		if (fmt.type == 'c')
			strlen = 1;
		else if (fmt.type == 's' && fmt.precision != -1 && fmt.precision < ft_strlen(str))
			strlen = fmt.precision;
		else
			strlen = ft_strlen(str);
		*total += strlen + ft_strlen(prefix);
		padlen = fmt.min_field_width - strlen - ft_strlen(prefix);
		pad = NULL;
		if (padlen > 0)
		{
			pad = make_pad(padlen, fmt);
			*total += padlen;
		}
		if (pad && !flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		{
			if (flag_is_set(fmt.flags, PAD_WITH_ZEROS) && fmt.precision == -1 && (fmt.type == 'd' || fmt.type == 'i' || flag_is_set(fmt.flags, ALTERNATE_FORM)))
			{
				write(1, prefix, ft_strlen(prefix));
				write(1, pad, padlen);
			}
			else
			{
				write(1, pad, padlen);
				write(1, prefix, ft_strlen(prefix));
			}
			write(1, str, strlen);
		}
		else if (pad && flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		{
			write(1, prefix, ft_strlen(prefix));
			write(1, str, strlen);
			write(1, pad, padlen);
		}
		else
		{
			write(1, prefix, ft_strlen(prefix));
			write(1, str, strlen);
		}
		free(pad);
	}
	else if (fmt.type == 's')
	{
		*total += 6;
		write(1, "(null)", 6);
	}
	if (fmt.type != 's')
		free(str);
	free(prefix);
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
			f = parse_format(cur);
			put(f, ap, &total);
			cur += f.specifier_length;
			start = cur;
		}
		else
			cur++;
	}
	write(1, start, cur - start);
	total += cur - start;
	va_end(ap);
	return (total);
}
