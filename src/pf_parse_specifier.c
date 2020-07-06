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

static const char	*parse_size_modifier(const char *pos, t_fmt *fmt)
{
	if (*pos == 'h' && *(pos + 1) == 'h')
	{
		pos += 2;
		fmt->size = Size_hh;
	}
	else if (*pos == 'h')
	{
		pos++;
		fmt->size = Size_h;
	}
	else if (*pos == 'l' && *(pos + 1) == 'l')
	{
		pos += 2;
		fmt->size = Size_ll;
	}
	else if (*pos == 'l')
	{
		pos++;
		fmt->size = Size_l;
	}
	else if (*pos == 'L')
	{
		pos++;
		fmt->size = Size_longdouble;
	}
	else
		fmt->size = Size_normal;
	return (pos);
}

static const char	*parse_a_number(const char *pos, int *value)
{
	*value = pf_simple_atoi(pos);
	while (*pos >= '0' && *pos <= '9')
		pos++;
	return (pos);
}

static char			choose_padchar(const t_fmt *fmt)
{
	char	padchar;

	if (fmt->pad_with_zero && !fmt->left_justify)
		padchar = '0';
	else
		padchar = ' ';
	if (fmt->pad_with_zero && fmt->precision != 1
		&& (fmt->type == Type_signed_d
			|| fmt->type == Type_signed_i
			|| fmt->type == Type_unsigned
			|| fmt->type == Type_octal
			|| fmt->type == Type_hex
			|| fmt->type == Type_hex_uppercase)
		)
		padchar = ' ';
	return (padchar);
}

static const char	*parse_min_width(const char *pos, t_fmt *fmt, va_list ap)
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

static const char	*parse_precision(const char *pos, t_fmt *fmt, va_list ap)
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

/*
** the characters in the type_symbols string must correspond to
** all of the conversion types in the enum e_type
** up to (but excluding) Type_none
*/

static const char	*parse_type(const char *pos, t_fmt *fmt)
{
	const char	*type_symbols;
	int			type_index;

	type_symbols = "%cspdiuoxXf";
	type_index = pf_strget_index(type_symbols, *pos);
	fmt->type = type_index >= 0 ? type_index : Type_none;
	if (fmt->type != Type_none)
		pos++;
	return (pos);
}

t_fmt				pf_parse_specifier(const char *str, va_list ap)
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
	pos = parse_size_modifier(pos, &fmt);
	pos = parse_type(pos, &fmt);
	fmt.padchar = choose_padchar(&fmt);
	fmt.spec_length = pos - str;
	return (fmt);
}
