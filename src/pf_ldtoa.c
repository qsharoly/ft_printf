/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_ldtoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/13 08:13:24 by qsharoly         ###   ########.fr       */
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

static char	*finalize(char *digits, int decimal_place, int precision, char sign_prefix, int alt_form)
{
	char			*ipart;
	char			*fpart;
	char			*s;
	int				i;
	int				fsize;

	i = ft_strlen(digits);
	if (decimal_place < 0)
	{
		decimal_place = -decimal_place;
		fsize =	ft_i_max(decimal_place, precision);
		fpart = malloc(fsize + 1);
		ft_memset(fpart, '0', fsize);
		fpart[fsize] = '\0';
		ft_memcpy(fpart, digits + i - decimal_place, decimal_place);
		if (decimal_place >= i)
		{
			ipart = ft_strdup("0");
			ft_memset(fpart, '0', decimal_place - i);
		}
		else
		{
			ipart = malloc(i - decimal_place);
			ft_memcpy(ipart, digits, (i - decimal_place));
		}
	}
	else
	{
		fpart = malloc(precision + 1);
		fpart[precision] = '\0';
		ft_memset(fpart, '0', precision);
		ipart = malloc(i + decimal_place + 1);
		ipart[i + decimal_place] = '\0';
		ft_memcpy(ipart, digits, i);
		ft_memset(ipart + i, '0', decimal_place);
	}
	fpart[precision] = '\0';
	s = malloc((sign_prefix != 0) + ft_strlen(ipart) + (precision != 0 || alt_form) * ft_strlen(".") + ft_strlen(fpart) + 1);
	if (sign_prefix != 0)
		s[0] = sign_prefix;
	ft_strcpy(s + (sign_prefix != 0), ipart);
	if (precision != 0 || alt_form)
		ft_strcat(s, ".");
	ft_strcat(s, fpart);
	free(ipart);
	free(fpart);
	return (s);
}

#if 0
	#include <stdio.h>
#endif
char	*pf_ldtoa(long double nb, int precision, const t_fmt *fmt)
{
	union f80 d;
	long	exponent;
	unsigned long	mantissa;
	int		is_subnormal;
	long	dec_pow;
	t_big	big;
	char	*s;

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
	s = finalize(big_to_string_round(big, -(dec_pow + precision)), dec_pow, precision, sign_prefix(d.d < 0, fmt), fmt->alternative_form);
	return (s);
}
