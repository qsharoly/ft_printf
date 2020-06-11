/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/26 08:24:36 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"
#include "libft.h"
#include "libftprintf.h"

static char	sign_prefix(int is_negative, const t_fmt *fmt)
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

static void	repeat(char c, int times, t_buffer *out)
{
	while (times-- > 0)
		pf_putc(c, out);
}

#include <stdio.h>
static void	put_float(const char *digits, int split_point, char sign_prefix, const t_fmt *fmt, t_buffer *out)
{
	int		pad_len;
	int		digits_len;
	int		prec;
	int		include_dot;
	int		i;

	prec = fmt->has_precision ? fmt->precision : DTOA_DEFAULT_PRECISION;
	include_dot = (prec > 0 || fmt->alternative_form);
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
	/*
	if (!fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
	if (sign_prefix != 0)
		pf_putc(sign_prefix, out);
	pf_puts(ipart, out);
	if (prec != 0 || fmt->alternative_form)
		pf_putc('.', out);
	pf_puts(fpart, out);
	if (fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
	return (s);
	*/
}

#if 0
	#include <stdio.h>
#endif
void	pf_dtoa(t_buffer *out, double nb, int precision, const t_fmt *fmt)
{
	union f64		d;
	long			exponent;
	unsigned long	mantissa;
	int				is_subnormal;
	long			dec_pow;
	t_big			big;
	char			*digits;

	d.d = nb;
	exponent = d.bits.exponent;
	mantissa = d.bits.mantissa;
#if 0
	printf("extracted: exponent = %ld, mantissa = %lu\n", exponent, mantissa);
#endif
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
#if 0
	printf("shifted mantissa = %lu, unbiased exponent = %ld, power = %ld\n", mantissa, exponent, dec_pow);
	printf("digits: %s * 10^%ld\n", big_to_string(big), dec_pow);
	printf("rounded: %s * 10^%ld\n", big_to_string_round(big, -(dec_pow + precision)), dec_pow);
#endif
	digits = big_to_string_round(big, -(dec_pow + precision));
	put_float(digits, ft_strlen(digits) + dec_pow, sign_prefix(d.d < 0, fmt), fmt, out);
}

void	pf_ldtoa(t_buffer *out, long double nb, int precision, const t_fmt *fmt)
{
	union f80 d;
	long	exponent;
	unsigned long	mantissa;
	int		is_subnormal;
	long	dec_pow;
	t_big	big;
	char	*digits;

	d.d = nb;
	exponent = d.bits.exponent;
	mantissa = d.bits.mantissa;
	is_subnormal = (exponent == 0);
	exponent = is_subnormal + exponent - F80_BIAS;
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
#if 0
	printf("shifted mantissa = %s, exponent = %ld, power = %ld\n", big_to_string(mantissa), exponent, dec_pow);
	printf("digits: %s * 10^%ld\n", big_to_string(big), dec_pow);
	printf("rounded: %s * 10^%ld\n", big_to_string_round(big, -(dec_pow + precision)), dec_pow);
#endif
	digits = big_to_string_round(big, -(dec_pow + precision));
	put_float(digits, ft_strlen(digits) + dec_pow, sign_prefix(d.d < 0, fmt), fmt, out);
}
