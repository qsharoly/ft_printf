/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_parse_specifier.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 13:26:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/12 05:49:49 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

static void (*g_conv_funcs[11])(char **, const t_fmt *, va_list) = {
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

static void		(*choose_conv(char type))(char **, const t_fmt *, va_list)
{
	int		i;

	i = pf_strchr_idx(type, "%cspdiuoxXf");
	if (i >= 0)
		return (g_conv_funcs[i]);
	else
		return (default_conv);
}

static char		*parse_flags(char *pos, t_fmt *fmt)
{
	while (ft_strchr("0-+ #", *pos))
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

static char		*parse_length_modifier(char *pos, t_fmt *fmt)
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
	else
		fmt->is_int = 1;
	return (pos);
}

static char		*parse_a_number(char *pos, int *value)
{
	*value = pf_simple_atoi(pos);
	while (*pos >= '0' && *pos <= '9')
		pos++;
	return (pos);
}

t_fmt			pf_parse_specifier(const char *str)
{
	t_fmt	fmt;
	char	*pos;

	pos = (char *)str + 1;
	ft_bzero((char *)&fmt, sizeof(t_fmt));
	fmt.precision = 1;
	pos = parse_flags(pos, &fmt);
	pos = parse_a_number(pos, &fmt.min_width);
	if (*pos == '.')
	{
		pos++;
		pos = parse_a_number(pos, &fmt.precision);
		fmt.has_precision = 1;
	}
	pos = parse_length_modifier(pos, &fmt);
	if (ft_strchr("%cspdiuoxXf", *pos))
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
