/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_specifier_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 13:26:37 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/21 19:21:06 by debby            ###   ########.fr       */
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
		fmt->size = Size_hh;
	else if (*pos == 'h')
		fmt->size = Size_h;
	else if (*pos == 'l' && *(pos + 1) == 'l')
		fmt->size = Size_ll;
	else if (*pos == 'l')
		fmt->size = Size_l;
	else if (*pos == 'L')
		fmt->size = Size_longdouble;
	else
		fmt->size = Size_normal;
	if (fmt->size == Size_hh || fmt->size == Size_ll)
		pos += 2;
	else if (fmt->size == Size_h || fmt->size == Size_l
			|| fmt->size == Size_longdouble)
		pos += 1;
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
		&& (fmt->conv == Conv_signed_d
			|| fmt->conv == Conv_signed_i
			|| fmt->conv == Conv_unsigned
			|| fmt->conv == Conv_octal
			|| fmt->conv == Conv_hex
			|| fmt->conv == Conv_hex_uppercase))
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
		fmt->min_width = ft_abs(nb);
	}
	else
		pos = parse_a_number(pos, &fmt->min_width);
	return (pos);
}

static const char	*parse_precision(const char *pos, t_fmt *fmt, va_list ap)
{
	int		prec;

	if (*pos == '.')
	{
		pos++;
		if (*pos == '*')
		{
			pos++;
			prec = va_arg(ap, int);
			if (prec < 0)
				return (pos);
		}
		else
			pos = parse_a_number(pos, &prec);
		fmt->precision = prec;
		fmt->has_precision = 1;
	}
	return (pos);
}

static const char	*parse_conv(const char *pos, t_fmt *fmt)
{
	if (*pos == '%')
		fmt->write_arg = conv_percent;
	else if (*pos == 'c')
		fmt->write_arg = conv_char;
	else if (*pos == 's')
		fmt->write_arg = conv_str;
	else if (*pos == 'p')
		fmt->write_arg = conv_ptr;
	else if (*pos == 'i' || *pos == 'd')
		fmt->write_arg = conv_signed;
	else if (*pos == 'u')
		fmt->write_arg = conv_unsigned;
	else if (*pos == 'o')
		fmt->write_arg = conv_oct;
	else if (*pos == 'x' || *pos == 'X')
		fmt->write_arg = conv_hex;
	else if (*pos == 'f')
		fmt->write_arg = conv_floating;
	else
	{
		fmt->write_arg = conv_default_noop;
		return (pos);
	}
	pos++;
	return (pos);
}

t_fmt				pf_specifier_parse(const char *str, va_list ap)
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
	pos = parse_conv(pos, &fmt);
	fmt.padchar = choose_padchar(&fmt);
	fmt.spec_length = pos - str;
	return (fmt);
}
