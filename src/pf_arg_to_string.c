/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_arg_to_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:24:14 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/14 06:43:41 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

static char		choose_padchar(const t_fmt *fmt)
{
	char	padchar;

	if (fmt->pad_with_zero && !fmt->left_justify)
		padchar = '0';
	else
		padchar = ' ';
	if (fmt->pad_with_zero && (ft_strchr("diuoxX", fmt->type))
			&& fmt->precision != 1)
		padchar = ' ';
	return (padchar);
}

static t_fatstr	fatstr_empty(void)
{
	t_fatstr	out;

	out.data = NULL;
	out.len = 0;
	return (out);
}

static t_fatstr	copy_with_pad(char *value_string, int value_len,
					int pad_len, const t_fmt *fmt)
{
	t_fatstr	out;
	char		padchar;

	out.len = value_len + pad_len;
	out.data = ft_strnew(out.len);
	padchar = choose_padchar(fmt);
	if (fmt->left_justify)
	{
		ft_memcpy(out.data, value_string, value_len);
		ft_memset(out.data + value_len, padchar, pad_len);
	}
	else
	{
		ft_memset(out.data, padchar, pad_len);
		ft_memcpy(out.data + pad_len, value_string, value_len);
	}
	return (out);
}

t_fatstr		pf_arg_to_string(const t_fmt *fmt, va_list ap)
{
	t_fatstr	out;
	char		*value_str;
	int			value_len;
	int			pad_len;

	value_str = NULL;
	fmt->to_string(&value_str, fmt, ap);
	if (fmt->type == 's' && value_str == NULL)
		value_str = "(null)";
	if (value_str == NULL)
		return (fatstr_empty());
	value_len = ft_strlen(value_str);
	if (fmt->type == 's' && fmt->has_precision && fmt->precision < value_len)
		value_len = fmt->precision;
	if (fmt->type == 'c')
		value_len = 1;
	pad_len = ft_i_max(fmt->min_width - value_len, 0);
	out = copy_with_pad(value_str, value_len, pad_len, fmt);
	if (fmt->type != 's')
		free(value_str);
	return (out);
}
