/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_conv_floating.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/20 21:10:52 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "float.h"
#include <limits.h>

static char	sign_char(int is_negative, const t_fmt *fmt)
{
	if (is_negative)
		return ('-');
	else if (fmt->prepend_plus)
		return ('+');
	else if (fmt->prepend_space)
		return (' ');
	else
		return ('\0');
}

static int	put_if_special(t_stream *out, t_fmt *fmt, long double nb)
{
	char	*s;
	char	sign;
	int		pad_len;

	s = NULL;
	if (ft_isnan(nb))
		s = "nan";
	else if (ft_isinf(nb))
		s = "inf";
	if (s == NULL)
		return (0);
	sign = sign_char(ft_isneg(nb), fmt);
	pad_len = fmt->min_width - ((sign != '\0') + ft_strlen(s));
	if (fmt->left_justify == 0)
		pf_repeat(' ', pad_len, out);
	if (sign)
		pf_putc(sign, out);
	pf_puts(s, out);
	if (fmt->left_justify)
		pf_repeat(' ', pad_len, out);
	return (1);
}

void		conv_floating(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	if (fmt->has_precision == 0)
		fmt->precision = DTOA_DEFAULT_PRECISION;
	if (put_if_special(out, fmt, arg.as_f))
		return ;
	if (ft_fabs(arg.as_f) < (long double)ULONG_MAX && fmt->precision < 20)
		pf_dtoa_quick(out, arg.as_f, fmt);
	else
		pf_dtoa(out, arg.as_f, fmt);
	/*
	pf_dtoa(out, arg.as_f, fmt);
		*/
}
