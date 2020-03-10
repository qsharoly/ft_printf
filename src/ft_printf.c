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

static void	pf_strnjoin(t_fat_string *tgt, const char *src, int size)
{
	char	*fresh;

	fresh = ft_strnew(tgt->len + size);
	if (fresh)
	{
		ft_memcpy(fresh, tgt->data, tgt->len);
		ft_memcpy(fresh + tgt->len, src, size);
	}
	tgt->len += size;
	free(tgt->data);
	tgt->data = fresh;
}

static t_fat_string	arg_to_string(t_fmt fmt, va_list ap)
{
	t_fat_string	out;
	char	*str;
	char	*pad;
	char	*prefix;
	int		padlen;
	int		strlen;

	out.data = ft_strnew(0);
	out.len = 0;
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
				pf_strnjoin(&out, prefix, ft_strlen(prefix));
			if (str)
				pf_strnjoin(&out, str, strlen);
			if (pad)
				pf_strnjoin(&out, pad, padlen);
		}
		else
		{
			if (fmt.pad_with_zero && fmt.precision == 1 && (fmt.type == 'd' || fmt.type == 'i' || fmt.alternative_form))
			{
				if (prefix)
					pf_strnjoin(&out, prefix, ft_strlen(prefix));
				if (pad)
					pf_strnjoin(&out, pad, padlen);
			}
			else
			{
				if (pad)
					pf_strnjoin(&out, pad, padlen);
				if (prefix) 
					pf_strnjoin(&out, prefix, ft_strlen(prefix));
			}
			if (str)
				pf_strnjoin(&out, str, strlen);
		}
	}
	if (fmt.type != 's')
		free(str);
	free(pad);
	free(prefix);
	return (out);
}

static t_list	*convert_split(const char *format, va_list ap)
{
	t_list	*parts;
	t_list	*elem;
	t_fat_string	s;
	char	*cur;
	t_fmt	f;

	parts = NULL;
	cur = (char *)format;
	while (*cur)
	{
		if (*cur == '%')
		{
			elem = ft_lstnew(format, cur - format);
			ft_lst_push_tail(&parts, elem);
			f = parse_format(cur);
			s = arg_to_string(f, ap);
			elem = ft_lstnew(s.data, s.len);
			free(s.data);
			ft_lst_push_tail(&parts, elem);
			cur += f.spec_length;
			format = cur;
		}
		else
			cur++;
	}
	elem = ft_lstnew(format, cur - format);
	ft_lst_push_tail(&parts, elem);
	return (parts);
}

static t_fat_string		concat(t_list *parts)
{
	t_list	*tmp;
	int		i;
	t_fat_string	s;

	tmp = parts;
	s.len = 0;
	while (tmp)
	{
		s.len += tmp->content_size;
		tmp = tmp->next;
	}
	s.data = (char *)malloc(sizeof(*s.data) * (s.len + 1));
	s.data[s.len] = '\0';
	i = 0;
	while (parts)
	{
		ft_memcpy(&s.data[i], parts->content, parts->content_size);
		i += parts->content_size;
		parts = parts->next;
	}
	return (s);
}

/*
** (void)size; suppresses unused parameter warning
*/
static void		pf_del(void *content, size_t size)
{
	(void)size;
	free(content);
}

int				ft_printf(const char * format, ...)
{
	va_list	ap;
	t_list	*parts;
	t_fat_string	s;
	int		total;

	va_start(ap, format);
	parts = convert_split(format, ap);
	va_end(ap);
	s = concat(parts);
	ft_lstdel(&parts, pf_del);
	total = write(1, s.data, s.len);
	free(s.data);
	if (total < 0)
		pf_error("write error\n");
	return (total);
}
