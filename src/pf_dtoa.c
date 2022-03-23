/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2022/03/23 21:58:49 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "bignum.h"
#include "float.h"
#include <limits.h>

static void	digits_put(t_sv digits, int split_offset, t_sv sign, const t_fmt *fmt, t_stream *out)
{
	int		pad_len;
	t_sv	dot;
	int		i;
	int		prec;
	int		split;

	dot = (fmt->precision > 0 || fmt->alternative_form) ? sv_from_cstr(".") : sv_from_cstr("");
	prec = fmt->precision;
	i = ft_max(1, ft_min(digits.length, split_offset));
	pad_len = fmt->min_width - (sign.length + i + dot.length + ft_min(digits.length - split_offset, prec));
	if (fmt->add_leading_zeros)
		put_sv(sign, out);
	put_repeat(fmt->padchar, (fmt->align == AlignRight) * pad_len, out);
	if (!fmt->add_leading_zeros)
		put_sv(sign, out);
	if (split_offset <= 0)
	{
		pf_putc('0', out);
		put_sv(dot, out);
		while (split_offset++ < 0 && prec-- > 0)
			pf_putc('0', out);
		digits.length = ft_min(digits.length, prec);
		put_sv(digits, out);
		prec -= digits.length;
		put_repeat('0', prec, out);
	}
	else
	{
		split = ft_min(digits.length, split_offset);
		i = 0;
		while (i < split)
			pf_putc(digits.start[i++], out);
		put_sv(dot, out);
		while (i < digits.length && prec-- > 0)
			pf_putc(digits.start[i++], out);
		put_repeat('0', prec, out);
	}
	put_repeat(fmt->padchar, (fmt->align == AlignLeft) * pad_len, out);
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
	char			buf[BIG_MAX_CHARS + 1];
	t_big			pow5;

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
		pow5 = big_raise(5, -dec_pow);
		big = big_mul(big_from_number(mantissa), pow5);
		big = big_mul(big, big_raise(2, exponent));
		digits = big_str(buf, big);
		digits = digits_round(digits, ft_strlen(digits) + dec_pow, fmt->precision);
	}
	if (digits)
		digits_put((t_sv){ digits, ft_strlen(digits) }, ft_strlen(digits) + dec_pow,
				sign_prefix(ft_isneg(nb), fmt), fmt, out);
}
