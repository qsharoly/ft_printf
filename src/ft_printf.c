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

#include <unistd.h>
#include "libft.h"
#include "libftprintf.h"

static void (*g_conversions[10])(char **, char **, t_fmt, va_list) = {
	percent_conv,
	c_conv,
	s_conv,
	p_conv,
	signed_conv,
	signed_conv,
	unsigned_conv,
	octal_conv,
	hex_conv,
	hex_conv
};

void	pf_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
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

static int	pf_strchr_index(char needle, const char *hay)
{
	int		i;

	i = 0;
	while (*hay)
	{
		if (*hay == needle)
			return (i);
		hay++;
		i++;
	}
	return (-1);
}

static void	(*choose_conv(char type))(char **, char **, t_fmt, va_list)
{
	int		i;

	i = pf_strchr_index(type, "%cspdiuoxX");
	if (i >= 0)
		return (g_conversions[i]);
	else
		return (default_conv);
}

/*
** repeated flag symbols are skipped
*/

static t_fmt	parse_format(const char *str)
{
	t_fmt	fmt;
	char	*ptr;

	ptr = (char *)str + 1;
	ft_bzero((char *)&fmt, sizeof(t_fmt));
	fmt.precision = 1;
	while (char_in_str(*ptr, "0-+ #"))
	{
		if (*ptr == '0')
			fmt.pad_with_zero = 1;
		else if (*ptr == '-')
			fmt.left_justify = 1;
		else if (*ptr == '+')
			fmt.prepend_plus = 1;
		else if (*ptr == ' ')
			fmt.prepend_space = 1;
		else if (*ptr == '#')
			fmt.alternative_form = 1;
		ptr++;
	}
	fmt.min_field_width = pf_simple_atoi(ptr);
	while (*ptr >= '0' && *ptr <= '9')
		ptr++;
	if (*ptr == '.')
	{
		ptr++;
		fmt.precision = pf_simple_atoi(ptr);
		fmt.has_precision = 1;
		while (*ptr >= '0' && *ptr <= '9')
			ptr++;
	}
	if (*ptr == 'h' && *(ptr + 1) == 'h')
	{
		ptr += 2;
		fmt.is_char = 1;
	}
	else if (*ptr == 'h')
	{
		ptr++;
		fmt.is_short = 1;
	}
	else if (*ptr == 'l' && *(ptr + 1) == 'l')
	{
		ptr += 2;
		fmt.is_longlong = 1;
	}
	else if (*ptr == 'l')
	{
		ptr++;
		fmt.is_long = 1;
	}
	else
		fmt.is_int = 1;
	if (char_in_str(*ptr, "%cspdiuoxX"))
	{
		fmt.type = *ptr;
		ptr++;
	}
	else
		fmt.type = TYPE_MISSING;
	fmt.to_string = choose_conv(fmt.type);
	fmt.spec_length = ptr - str;
	return (fmt);
}

static char *make_pad(int padlen, t_fmt fmt)
{
	int		i;
	char	*pad;
	char	padchar;

	if (padlen <= 0)
		return (NULL);
	if (fmt.pad_with_zero && !fmt.left_justify)
		padchar = '0';
	else
		padchar = ' ';
	if (fmt.pad_with_zero && (char_in_str(fmt.type, "diuoxX")) && fmt.precision != 1)
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

/*
** This still works when there is no pad, because write does nothing
** if passed a NULL pointer.
** For 'c' type strlen is always 1 so that we do print a '\0' character.
*/

static void	put(t_fmt fmt, va_list ap, int *total)
{
	char	*str;
	char	*pad;
	char	*prefix;
	int		padlen;
	int		strlen;

	str = NULL;
	prefix = NULL;
	pad = NULL;
	fmt.to_string(&str, &prefix, fmt, ap);
	if (fmt.type == 's' && str == NULL)
		str = "(null)";
	if (str)
	{
		strlen = ft_strlen(str);
		if (fmt.type == 's' && fmt.has_precision && fmt.precision < strlen)
			strlen = fmt.precision;
		if (fmt.type == 'c')
			strlen = 1;
		if (prefix)
			padlen = fmt.min_field_width - strlen - ft_strlen(prefix);
		else
			padlen = fmt.min_field_width - strlen;
		if (padlen > 0)
			pad = make_pad(padlen, fmt);
		else
			padlen = 0;
		if (fmt.left_justify)
		{
			if (prefix)
				write(1, prefix, ft_strlen(prefix));
			if (str)
				write(1, str, strlen);
			if (pad)
				write(1, pad, padlen);
		}
		else
		{
			if (fmt.pad_with_zero && fmt.precision == 1 && (fmt.type == 'd' || fmt.type == 'i' || fmt.alternative_form))
			{
				if (prefix)
					write(1, prefix, ft_strlen(prefix));
				if (pad)
					write(1, pad, padlen);
			}
			else
			{
				if (pad)
					write(1, pad, padlen);
				if (prefix) 
					write(1, prefix, ft_strlen(prefix));
			}
			if (str)
				write(1, str, strlen);
		}
		*total += padlen + strlen;
		if (prefix)
			*total += ft_strlen(prefix);
	}
	if (fmt.type != 's')
		free(str);
	free(pad);
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
			cur += f.spec_length;
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
