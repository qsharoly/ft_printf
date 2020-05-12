/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/12 23:36:35 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"
#include "libft.h"

#define F64_EXPONENT_MASK 0x7ff0000000000000
#define F64_MANTISSA_MASK 0x000fffffffffffff
#define F64_EXPONENT_SIZE 11
#define F64_MANTISSA_SIZE 52
#define F64_EXPONENT_BIAS 1023

long	get_exponent(double a)
{
	long	e;

	e = (*(long *)&a & F64_EXPONENT_MASK) >> F64_MANTISSA_SIZE;
	return (e);
}

long	get_mantissa(double a)
{
	long	m;

	m = *(long *)&a & F64_MANTISSA_MASK;
	return (m);
}

char	*finalize(char *digits, int decimal_place, int precision, int is_neg)
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
	s = malloc(is_neg + ft_strlen(ipart) + ft_strlen(".") + ft_strlen(fpart) + 1);
	ft_strcpy(s + is_neg, ipart);
	if (is_neg)
		s[0] = '-';
	ft_strcat(s, ".");
	ft_strcat(s, fpart);
	free(ipart);
	free(fpart);
	return (s);
}

char	*pf_dtoa(double d, int precision)
{
	long			exponent;
	unsigned long	mantissa;
	int				is_subnormal;
	long			dec_pow;
	t_big			big;
	char			*s;

	exponent = get_exponent(d);
	mantissa = get_mantissa(d);
	is_subnormal = (exponent == 0);
	exponent = is_subnormal + exponent - F64_EXPONENT_BIAS;
	mantissa = mantissa + (!is_subnormal) * (1L << F64_MANTISSA_SIZE);
	dec_pow = -F64_MANTISSA_SIZE;
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
	printf("shifted mantissa = %lu, exponent = %ld, power = %ld\n", mantissa, exponent, dec_pow);
	printf("integerized form: %s * 10^%ld\n", big_to_string(big), dec_pow);
#endif
	s = finalize(big_to_string(big), dec_pow, precision, (d < 0));
	return (s);
}
