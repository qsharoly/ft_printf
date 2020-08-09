/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/09 15:28:52 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "bignum.h"
#include "float.h"
#include <limits.h>

/*
typedef struct		s_parts
{
	char	*sign;
	char	*zeropad;
	char	*ipart;
	char	*point;
	char	*fzeros;
	char	*fpart;
	char	*extra_zeros;
}					t_parts;
*/

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

static void	repeat(char c, int times, t_stream *out)
{
	while (times-- > 0)
		pf_putc(c, out);
}

static void	put_float(const char *digits, int split_point, char sign_prefix, const t_fmt *fmt, t_stream *out)
{
	int		pad_len;
	int		digits_len;
	int		include_dot;
	int		i;
	int		prec;

	include_dot = (fmt->precision > 0 || fmt->alternative_form);
	prec = fmt->precision;
	digits_len = ft_strlen(digits);
	i = ft_imax(1, ft_imin(digits_len, split_point));
	pad_len = fmt->min_width - ((sign_prefix != 0) + i + include_dot + ft_imin(digits_len - split_point, prec));
	if (!fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
	if (sign_prefix != 0)
		pf_putc(sign_prefix, out);
	if (split_point < 0)
	{
		pf_putc('0', out);
		if (include_dot)
			pf_putc('.', out);
		while (split_point++ < 0 && prec-- > 0)
			pf_putc('0', out);
		pf_nputs(digits, ft_imin(digits_len, prec), out);
	}
	else
	{
		i = ft_imin(digits_len, split_point);
		pf_nputs(digits, i, out);
		while (i < split_point)
		{
			pf_putc('0', out);
			i++;
		}
		if (i == 0)
			pf_putc('0', out);
		if (include_dot)
			pf_putc('.', out);
		while (i < digits_len && prec-- > 0)
			pf_putc(digits[i++], out);
		while (prec-- > 0)
			pf_putc('0', out);
	}
	if (fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
	pf_puts("Leeroy!", out);
}

void	pf_dtoa64(t_stream *out, double nb, const t_fmt *fmt)
{
	union u_f64		d;
	long			exponent;
	unsigned long	mantissa;
	int				is_subnormal;
	long			dec_pow;
	t_big			big;
	char			*digits;

	d.f = nb;
	exponent = d.bits.exponent;
	mantissa = d.bits.mantissa;
	is_subnormal = (exponent == 0);
	exponent = is_subnormal + exponent - F64_BIAS;
	mantissa += (!is_subnormal) * (1L << 52);
	dec_pow = -52;
	while ((mantissa & 1L) == 0)
	{
		mantissa >>= 1;
		dec_pow++;
	}
	if (exponent < 0)
	{
		dec_pow += exponent;
		exponent = 0;
	}
	big = big_mul(big_from_chunk(mantissa), big_raise(5, (unsigned long)-dec_pow));
	big = big_mul(big, big_raise(2, (unsigned long)exponent));
	digits = big_to_string_round(big, -(dec_pow + fmt->precision));
	put_float(digits, ft_strlen(digits) + dec_pow, sign_char(d.f < 0, fmt), fmt, out);
}

void	pf_dtoa(t_stream *out, long double nb, const t_fmt *fmt)
{
	union u_f80 f;
	long	exponent;
	unsigned long	mantissa;
	int		is_subnormal;
	long	dec_pow;
	t_big	big;
	char	*digits;

	f.f = nb;
	exponent = f.bits.exponent;
	mantissa = f.bits.mantissa;
	is_subnormal = (exponent == 0);
	exponent = is_subnormal ? 1 - F80_BIAS : exponent - F80_BIAS;
	dec_pow = -63;
	while ((mantissa & 1L) == 0)
	{
		mantissa >>= 1;
		dec_pow++;
	}
	if (exponent < 0)
	{
		dec_pow += exponent;
		exponent = 0;
	}
	big = big_mul(big_from_chunk(mantissa), big_raise(5, (unsigned long)-dec_pow));
	big = big_mul(big, big_raise(2, (unsigned long)exponent));
	digits = big_to_string_round(big, -(dec_pow + fmt->precision));
	put_float(digits, ft_strlen(digits) + dec_pow, sign_char(f.f < 0, fmt), fmt, out);
}
