/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/18 07:41:43 by debby            ###   ########.fr       */
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
	i = ft_imax(1, ft_imin(digits_len, split_offset));
	pad_len = fmt->min_width - ((sign != 0) + i + (dot != 0) + ft_imin(digits_len - split_offset, prec));
	if (sign && fmt->pad_with_zero)
		pf_putc(sign, out);
	if (!fmt->left_justify)
		pf_repeat(/*'p'*/fmt->padchar, pad_len, out);
	if (sign && !fmt->pad_with_zero)
		pf_putc(sign, out);
	if (split_offset <= 0)
	{
		pf_putc('0', out);
		if (dot)
			pf_putc(dot, out);
		while (split_offset++ < 0 && prec-- > 0)
			pf_putc(/*'a'*/'0', out);
		pf_nputs(digits, ft_imin(digits_len, prec), out);
	}
	else
	{
		i = ft_imin(digits_len, split_offset);
		pf_nputs(digits, i, out);
		while (i < split_offset) //impossible?
		{
			pf_putc(/*'b'*/'0', out);
			i++;
		}
		if (dot)
			pf_putc(dot, out);
		while (i < digits_len && prec-- > 0)
			pf_putc(digits[i++], out);
		while (prec-- > 0)
			pf_putc(/*'c'*/'0', out);
	}
	if (fmt->left_justify)
		pf_repeat(/*'p'*/fmt->padchar, pad_len, out);
}

char	*digits_round(char *digits, int rounding_position)
{
	int	i;
	int	dig_len;

	dig_len = ft_strlen(digits);
	if (rounding_position < 0)
		rounding_position += dig_len;
	if (digits[rounding_position] > '4')
	{
		if (digits[rounding_position] == '5')
		{
			i = rounding_position + 1;
			while(i < dig_len && digits[i] == '0')
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
		if (i < 0 && dig_len >= BIG_TO_STR_BUFSIZE - 1)
		{
			ft_putstr_fd("digits buffer overflow when rounding\n", 2);
			return (NULL);
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

void	pf_dtoa(t_stream *out, long double nb, const t_fmt *fmt)
{
	union u_f80		f;
	long			exponent;
	unsigned long	mantissa;
	int				is_subnormal;
	long			dec_pow;
	t_big			big;
	char			*digits;
	char			buf[BIG_TO_STR_BUFSIZE];

	f.f = nb;
	exponent = f.bits.exponent;
	mantissa = f.bits.mantissa;
	is_subnormal = (exponent == 0);
	exponent = is_subnormal ? 1 - F80_BIAS : exponent - F80_BIAS;
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
		big = big_mul(big_from_chunk(mantissa), big_raise(5, (unsigned long)-dec_pow));
		big = big_mul(big, big_raise(2, (unsigned long)exponent));
		digits = digits_round(big_str(buf, big), dec_pow + fmt->precision);
	}
	if (digits)
		digits_put(digits, ft_strlen(digits) + dec_pow,
				sign_char(ft_isneg(nb), fmt), fmt, out);
}
