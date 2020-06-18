/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 22:46:27 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "bignum.h"
#include "float.h"
#include <limits.h>

static unsigned long	g_pow10[20] = {
	1L,
	10L,
	100L,
	1000L,
	10000L,
	100000L,
	1000000L,
	10000000L,
	100000000L,
	1000000000L,
	10000000000L,
	100000000000L,
	1000000000000L,
	10000000000000L,
	100000000000000L,
	1000000000000000L,
	10000000000000000L,
	100000000000000000L,
	1000000000000000000L,
	10000000000000000000UL
};

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

static void	put_float(const char *digits, int split_point, char sign_prefix, const t_fmt *fmt, t_buffer *out)
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

void	pf_putdbl_quick(t_buffer *out, double nb, const t_fmt *fmt)
{
	double	ipart;
	double	fpart;
	double	frem;
	char	buf[MAXBUF_ITOA];
	char	buf2[MAXBUF_ITOA];
	char	*i_start;
	char	*f_start;
	char	sign;
	int		pad_len;
	int		i;
	int		include_dot;


	sign = sign_prefix(nb < 0.0, fmt);
	include_dot = (fmt->precision > 0 || fmt->alternative_form);
	nb = ft_fabs(nb);
	ipart = ft_trunc(nb);
	fpart = ((nb - ipart) * g_pow10[fmt->precision]);
	frem = fpart - ft_trunc(fpart);
	if ((nb > 0 && frem >= 0.5) || (nb < 0 && frem > 0.5))
		fpart++;
	if (fpart > (double)g_pow10[fmt->precision])
	{
		ipart++;
		fpart = 0;
	}
	if (ipart == 0.0)
		i_start = "0";
	else
		i_start = pf_utoa_base(buf, (unsigned long)ipart, 10, 0);
	f_start = pf_utoa_base(buf2, (unsigned long)fpart, 10, 0);
	i = 0;
	while (fpart * g_pow10[i] < g_pow10[fmt->precision] && i <= fmt->precision)
		i++;
	i = i - 1;
	pad_len = fmt->min_width - ((sign != 0) + ft_strlen(i_start) + include_dot
			+ i + ft_strlen(f_start));
	if (!fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
	if (sign != 0)
		pf_putc(sign, out);
	pf_puts(i_start, out);
	if (include_dot)
		pf_putc('.', out);
	while (i-- > 0)
		pf_putc('0', out);
	pf_puts(f_start, out);
	if (fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
}

void	pf_put_longdbl_quick(t_buffer *out, long double nb, const t_fmt *fmt)
{
	long double	ipart;
	long double	fpart;
	long double frem;
	char	buf[MAXBUF_ITOA];
	char	buf2[MAXBUF_ITOA];
	char	*i_start;
	char	*f_start;
	char	sign;
	int		pad_len;
	int		i;
	int		include_dot;


	sign = sign_prefix(nb < 0.0, fmt);
	include_dot = (fmt->precision > 0 || fmt->alternative_form);
	nb = ft_fabsl(nb);
	ipart = ft_truncl(nb);
	fpart = ((nb - ipart) * (long double)g_pow10[fmt->precision]);
	frem = fpart - ft_truncl(fpart);
	if ((nb > 0 && frem >= 0.5) || (nb < 0 && frem > 0.5))
		fpart++;
	if (fpart > (long double)g_pow10[fmt->precision])
	{
		ipart++;
		fpart = 0;
	}
	if (ipart == 0.0)
		i_start = "0";
	else
		i_start = pf_utoa_base(buf, (unsigned long)ipart, 10, 0);
	f_start = pf_utoa_base(buf2, (unsigned long)fpart, 10, 0);
	i = 0;
	while (fpart * g_pow10[i] < g_pow10[fmt->precision] && i <= fmt->precision)
		i++;
	i = i - 1;
	pad_len = fmt->min_width - ((sign != 0) + ft_strlen(i_start) + include_dot
			+ i + ft_strlen(f_start));
	if (!fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
	if (sign != 0)
		pf_putc(sign, out);
	pf_puts(i_start, out);
	if (include_dot)
		pf_putc('.', out);
	while (i-- > 0)
		pf_putc('0', out);
	pf_puts(f_start, out);
	if (fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
}

void	pf_dtoa(t_buffer *out, double nb, const t_fmt *fmt)
{
	union u_f64		d;
	long			exponent;
	unsigned long	mantissa;
	int				is_subnormal;
	long			dec_pow;
	t_big			big;
	char			*digits;

	d.d = nb;
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
	put_float(digits, ft_strlen(digits) + dec_pow, sign_prefix(d.d < 0, fmt), fmt, out);
}

void	pf_ldtoa(t_buffer *out, long double nb, const t_fmt *fmt)
{
	union u_f80 d;
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
	put_float(digits, ft_strlen(digits) + dec_pow, sign_prefix(d.d < 0, fmt), fmt, out);
}
