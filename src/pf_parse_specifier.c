/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_parse_specifier.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 13:26:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 20:08:19 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

/*
** Number and order of conversion functions must correspond to
** the value of the string 'types' in choose_conv function.
*/

static void (*g_conv_funcs[11])(t_buffer *, t_fmt *, va_list) = {
	percent_conv,
	c_conv,
	s_conv,
	p_conv,
	signed_conv,
	signed_conv,
	unsigned_conv,
	octal_conv,
	hex_conv,
	hex_conv,
	double_conv
};

static void		(*choose_conv(char type))(t_buffer *, t_fmt *, va_list)
{
	int		i;
	char	*types;

	types = "%cspdiuoxXf";
	i = pf_strchr_idx(type, types);
	if (i >= 0)
		return (g_conv_funcs[i]);
	else
		return (default_conv);
}

static const char	*parse_flags(const char *pos, t_fmt *fmt)
{
	while (ft_strhas("0-+ #", *pos))
	{
		if (*pos == '0')
			fmt->pad_with_zero = 1;
		else if (*pos == '-')
			fmt->left_justify = 1;
		else if (*pos == '+')
			fmt->prepend_plus = 1;
		else if (*pos == ' ')
			fmt->prepend_space = 1;
		else if (*pos == '#')
			fmt->alternative_form = 1;
		pos++;
	}
	return (pos);
}

static const char	*parse_type_modifier(const char *pos, t_fmt *fmt)
{
	if (*pos == 'h' && *(pos + 1) == 'h')
	{
		pos += 2;
		fmt->is_char = 1;
	}
	else if (*pos == 'h')
	{
		pos++;
		fmt->is_short = 1;
	}
	else if (*pos == 'l' && *(pos + 1) == 'l')
	{
		pos += 2;
		fmt->is_longlong = 1;
	}
	else if (*pos == 'l')
	{
		pos++;
		fmt->is_long = 1;
	}
	else if (*pos == 'L')
	{
		pos++;
		fmt->is_quad = 1;
	}
	else
		fmt->is_int = 1;
	return (pos);
}

static const char	*parse_a_number(const char *pos, int *value)
{
	*value = pf_simple_atoi(pos);
	while (*pos >= '0' && *pos <= '9')
		pos++;
	return (pos);
}

static char		choose_padchar(const t_fmt *fmt)
{
	char	padchar;

	if (fmt->pad_with_zero && !fmt->left_justify)
		padchar = '0';
	else
		padchar = ' ';
	if (fmt->pad_with_zero && (ft_strhas("diuoxX", fmt->type))
			&& fmt->precision != 1)
		padchar = ' ';
	return (padchar);
}

const char		*parse_min_width(const char *pos, t_fmt *fmt, va_list ap)
{
	int		nb;

	if (*pos == '*')
	{
		pos++;
		nb = va_arg(ap, int);
		if (nb < 0)
			fmt->left_justify = 1;
		fmt->min_width = ft_iabs(nb);
	}
	else
		pos = parse_a_number(pos, &fmt->min_width);
	return (pos);
}

const char		*parse_precision(const char *pos, t_fmt *fmt, va_list ap)
{
	if (*pos == '.')
	{
		pos++;
		if (*pos == '*')
		{
			pos++;
			fmt->precision = va_arg(ap, int);
		}
		else
			pos = parse_a_number(pos, &fmt->precision);
		fmt->has_precision = 1;
	}
	return (pos);
}

t_fmt			pf_parse_specifier(const char *str, va_list ap)
{
	t_fmt		fmt;
	const char	*pos;

	pos = str + 1;
	ft_bzero((char *)&fmt, sizeof(t_fmt));
	fmt.precision = 1;
	pos = parse_flags(pos, &fmt);
	pos = parse_min_width(pos, &fmt, ap);
	if (ft_isdigit(*pos) || (*pos == '-' && ft_isdigit(*(pos + 1))))
		pos = parse_min_width(pos, &fmt, ap);
	pos = parse_precision(pos, &fmt, ap);
	pos = parse_type_modifier(pos, &fmt);
	if (ft_strhas("%cspdiuoxXf", *pos))
	{
		fmt.type = *pos;
		pos++;
	}
	else
		fmt.type = TYPE_MISSING;
	fmt.write_arg = choose_conv(fmt.type);
	fmt.padchar = choose_padchar(&fmt);
	fmt.spec_length = pos - str;
	return (fmt);
}
