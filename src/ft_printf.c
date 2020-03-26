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

static void (*g_conversions[10])(char **, t_fmt, va_list) = {
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

static int	strchr_index(char needle, const char *hay)
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

static void	(*choose_conv(char type))(char **, t_fmt, va_list)
{
	int		i;

	i = strchr_index(type, "%cspdiuoxX");
	if (i >= 0)
		return (g_conversions[i]);
	else
		return (default_conv);
}

static t_fmt	parse_specifier(const char *str)
{
	t_fmt	fmt;
	char	*pos;

	pos = (char *)str + 1;
	ft_bzero((char *)&fmt, sizeof(t_fmt));
	fmt.precision = 1;
	while (char_in_str(*pos, "0-+ #"))
	{
		if (*pos == '0')
			fmt.pad_with_zero = 1;
		else if (*pos == '-')
			fmt.left_justify = 1;
		else if (*pos == '+')
			fmt.prepend_plus = 1;
		else if (*pos == ' ')
			fmt.prepend_space = 1;
		else if (*pos == '#')
			fmt.alternative_form = 1;
		pos++;
	}
	fmt.min_width = pf_simple_atoi(pos);
	while (*pos >= '0' && *pos <= '9')
		pos++;
	if (*pos == '.')
	{
		pos++;
		fmt.precision = pf_simple_atoi(pos);
		fmt.has_precision = 1;
		while (*pos >= '0' && *pos <= '9')
			pos++;
	}
	if (*pos == 'h' && *(pos + 1) == 'h')
	{
		pos += 2;
		fmt.is_char = 1;
	}
	else if (*pos == 'h')
	{
		pos++;
		fmt.is_short = 1;
	}
	else if (*pos == 'l' && *(pos + 1) == 'l')
	{
		pos += 2;
		fmt.is_longlong = 1;
	}
	else if (*pos == 'l')
	{
		pos++;
		fmt.is_long = 1;
	}
	else
		fmt.is_int = 1;
	if (char_in_str(*pos, "%cspdiuoxX"))
	{
		fmt.type = *pos;
		pos++;
	}
	else
		fmt.type = TYPE_MISSING;
	fmt.to_string = choose_conv(fmt.type);
	fmt.spec_length = pos - str;
	return (fmt);
}

static char choose_padchar(t_fmt fmt)
{
	char	padchar;

	if (fmt.pad_with_zero && !fmt.left_justify)
		padchar = '0';
	else
		padchar = ' ';
	if (fmt.pad_with_zero && (char_in_str(fmt.type, "diuoxX")) && fmt.precision != 1)
		padchar = ' ';
	return (padchar);
}

static t_fatstr	arg_to_string(t_fmt fmt, va_list ap)
{
	t_fatstr	out;
	char		*str;
	char		padchar;
	int			padlen;
	int			strlen;

	out.data = NULL;
	out.len = 0;
	str = NULL;
	padchar = choose_padchar(fmt);
	fmt.to_string(&str, fmt, ap);
	if (fmt.type == 's' && str == NULL)
		str = "(null)";
	if (str == NULL)
		return (out);
	strlen = ft_strlen(str);
	if (fmt.type == 's' && fmt.has_precision && fmt.precision < strlen)
		strlen = fmt.precision;
	if (fmt.type == 'c')
		strlen = 1;
	padlen = fmt.min_width - strlen;
	if (padlen < 0)
		padlen = 0;
	out.len = strlen + padlen;
	out.data = ft_strnew(out.len);
	if (fmt.left_justify)
	{
		ft_memcpy(out.data, str, strlen);
		ft_memset(out.data + strlen, padchar, padlen);
	}
	else
	{
		ft_memset(out.data, padchar, padlen);
		ft_memcpy(out.data + padlen, str, strlen);
	}
	if (fmt.type != 's')
		free(str);
	return (out);
}

static t_list	*split_convert(const char *format, va_list ap)
{
	t_list		*parts;
	t_list		*elem;
	t_fatstr	s;
	t_fmt		fmt;
	char		*cur;

	parts = NULL;
	cur = (char *)format;
	while (*cur)
	{
		if (*cur == '%')
		{
			elem = ft_lstnew(format, cur - format);
			ft_lst_push_tail(&parts, elem);
			fmt = parse_specifier(cur);
			s = arg_to_string(fmt, ap);
			elem = ft_lstnew(s.data, s.len);
			ft_lst_push_tail(&parts, elem);
			free(s.data);
			cur += fmt.spec_length;
			format = cur;
		}
		else
			cur++;
	}
	elem = ft_lstnew(format, cur - format);
	ft_lst_push_tail(&parts, elem);
	return (parts);
}

static t_fatstr		concat(t_list *parts)
{
	t_list		*tmp;
	t_fatstr	whole;
	int			pos;

	tmp = parts;
	whole.len = 0;
	while (tmp)
	{
		whole.len += tmp->content_size;
		tmp = tmp->next;
	}
	whole.data = malloc(sizeof(*whole.data) * (whole.len + 1));
	whole.data[whole.len] = '\0';
	pos = 0;
	while (parts)
	{
		ft_memcpy(whole.data + pos, parts->content, parts->content_size);
		pos += parts->content_size;
		parts = parts->next;
	}
	return (whole);
}

/*
** size is unused
*/
static void		simple_del(void *content, size_t size)
{
	(void)size;
	free(content);
}

int				ft_printf(const char * format, ...)
{
	va_list		ap;
	t_list		*parts;
	t_fatstr	s;
	int			total;

	va_start(ap, format);
	parts = split_convert(format, ap);
	va_end(ap);
	s = concat(parts);
	ft_lstdel(&parts, simple_del);
	total = write(1, s.data, s.len);
	free(s.data);
	if (total < 0)
		pf_error("write error\n");
	return (total);
}
