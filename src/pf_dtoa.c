/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/23 01:42:10 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "bignum.h"
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
		return (0);
}

static void	digits_put(const char *digits, int split_offset, char sign, const t_fmt *fmt, t_stream *out)
{
	int		pad_len;
	int		digits_len;
	char	dot;
	int		i;
	int		prec;

	dot = (fmt->precision > 0 || fmt->alternative_form) ? '.' : 0;
	prec = fmt->precision;
	digits_len = ft_strlen(digits);
	i = ft_max(1, ft_min(digits_len, split_offset));
	pad_len = fmt->min_width - ((sign != 0) + i + (dot != 0) + ft_min(digits_len - split_offset, prec));
	if (sign && fmt->pad_with_zero)
		pf_putc(sign, out);
	pf_repeat(fmt->padchar, !fmt->left_justify * pad_len, out);
	if (sign && !fmt->pad_with_zero)
		pf_putc(sign, out);
	if (split_offset <= 0)
	{
		pf_putc('0', out);
		if (dot)
			pf_putc(dot, out);
		while (split_offset++ < 0 && prec-- > 0)
			pf_putc('0', out);
		pf_nputs(digits, ft_min(digits_len, prec), out);
		prec -= digits_len;
		while (prec-- > 0)
			pf_putc('0', out);
	}
	else
	{
		i = ft_min(digits_len, split_offset);
		pf_nputs(digits, i, out);
		if (dot)
			pf_putc(dot, out);
		while (i < digits_len && prec-- > 0)
			pf_putc(digits[i++], out);
		while (prec-- > 0)
			pf_putc('0', out);
	}
	pf_repeat(fmt->padchar, fmt->left_justify * pad_len, out);
}

char	*digits_round(char *digits, int split_offset, int precision)
{
	int	i;
	int	dig_len;
	int	rounding_position;

	dig_len = ft_strlen(digits);
	rounding_position = split_offset + precision;
	if (rounding_position < 0 || rounding_position > dig_len)
		return (digits);
	if (digits[rounding_position] > '4')
	{
		if (digits[rounding_position] == '5')
		{
			i = rounding_position + 1;
			while (i < dig_len && digits[i] == '0')
			{
				i++;
			}
			if (i == dig_len && (rounding_position == 0
					|| (digits[rounding_position - 1] - '0') % 2 == 0))
			{
				return (digits);
			}
		}
		i = rounding_position - 1;
		while (i >= 0 && digits[i] == '9')
		{
			digits[i] = '0';
			i--;
		}
		if (i < 0)
		{
			digits--;
			*digits = '1';
		}
		else
			digits[i] = digits[i] + 1;
	}
	return (digits);
}

static long	get_exponent(long double nb)
{
	union u_f80	f;

	f.f = nb;
	return (f.bits.exponent);
}

static unsigned long	get_mantissa(long double nb)
{
	union u_f80	f;

	f.f = nb;
	return (f.bits.mantissa);
}

void	pf_dtoa(t_stream *out, long double nb, const t_fmt *fmt)
{
	long			exponent;
	unsigned long	mantissa;
	long			dec_pow;
	t_big			big;
	char			*digits;
	char			buf[BIG_TO_STR_BUFSIZE];

	exponent = get_exponent(nb);
	mantissa = get_mantissa(nb);
	exponent = exponent == 0 ? 1 - F80_BIAS : exponent - F80_BIAS;
	dec_pow = -63;
	while ((mantissa & 1L) == 0 && mantissa > 0)
	{
		mantissa >>= 1;
		dec_pow++;
	}
	if (exponent < 0)
	{
		dec_pow += exponent;
		exponent = 0;
	}
	if (mantissa == 0)
	{
		digits = "0";
	}
	else
	{
		big = big_mul(big_from_number(mantissa), big_raise(5, -dec_pow));
		big = big_mul(big, big_raise(2, exponent));
		digits = big_str(buf, big);
		digits = digits_round(digits, ft_strlen(digits) + dec_pow, fmt->precision);
	}
	if (digits)
		digits_put(digits, ft_strlen(digits) + dec_pow,
				sign_char(ft_isneg(nb), fmt), fmt, out);
}
