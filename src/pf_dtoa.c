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
static void	put_float(char *digits, int decimal_power, char sign_prefix, const t_fmt *fmt, t_buffer *out)
{
	int		pad_len;
	int		dot_idx;
	int		fsize;
	int		prec;
	int		include_dot;

	prec = fmt->has_precision ? fmt->precision : DTOA_DEFAULT_PRECISION;
	include_dot = (prec > 0 || fmt->alternative_form);
	if (sign_prefix)
		pf_putc(sign_prefix, out);
	if (decimal_power < 0)
	{
		dot_idx = ft_strlen(digits) + decimal_power;
		if (dot_idx > 0)
		{
			/*
			 * number of form 12345.678900
			 */
			printf("path a");
			pad_len = fmt->min_width - (sign_prefix != 0 + dot_idx + include_dot + ft_imin(ft_strlen(digits + dot_idx), prec));
			if (!fmt->left_justify)
				repeat(fmt->padchar, pad_len, out);
			pf_nputs(digits, dot_idx, out);
			if (include_dot)
				pf_putc('.', out);
			fsize = ft_imin(ft_strlen(digits + dot_idx), prec);
			pf_nputs(digits + dot_idx, fsize, out);
			repeat('0', prec - fsize, out);
			if (fmt->left_justify)
				repeat(fmt->padchar, pad_len, out);
		}
		else
		{
			/*
			 * number of form 0.00012345
			 */
			//printf("digits = %s, prec = %d, dot_idx = %d, len = %ld, decimal_power = %d ", digits, prec, dot_idx, ft_strlen(digits), decimal_power);
			printf("path b");
			dot_idx = -dot_idx;
			pad_len = fmt->min_width - (1 + include_dot + ft_imin(prec, dot_idx) + ft_imax(0, prec - dot_idx));
			if (!fmt->left_justify)
				repeat(fmt->padchar, pad_len, out);
			pf_putc('0', out);
			if (include_dot)
				pf_putc('.', out);
			repeat('0', ft_imin(prec, dot_idx), out);
			pf_nputs(digits, ft_imax(0, prec - dot_idx), out);
			if (fmt->left_justify)
				repeat(fmt->padchar, pad_len, out);
		}
	}
	else
	{
		/*
		 * number of form 123450000.00
		 */
		printf("path c");
		pad_len = fmt->min_width - (ft_strlen(digits) + decimal_power + include_dot + prec);
		if (!fmt->left_justify)
			repeat(fmt->padchar, pad_len, out);
		pf_puts(digits, out);
		repeat('0', decimal_power, out);
		if (include_dot)
			pf_putc('.', out);
		repeat('0', prec, out);
		if (fmt->left_justify)
			repeat(fmt->padchar, pad_len, out);
	}
	/*
	if (sign_prefix != 0)
		pf_putc(sign_prefix, out);
	pf_puts(ipart, out);
	if (prec != 0 || fmt->alternative_form)
		pf_putc('.', out);
	pf_puts(fpart, out);
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
	put_float(big_to_string_round(big, -(dec_pow + precision)), dec_pow, sign_prefix(d.d < 0, fmt), fmt, out);
}
