/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_dtoa_quick.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/09 14:17:37 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
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

static char				sign_char(int is_negative, const t_fmt *fmt)
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

static void				part_and_round(t_parts *p, long double nb, int prec,
										enum e_size size)
{
	long double	rem;

	p->ipart = ft_trunc(ft_fabs(nb));
	p->fpart = ((ft_fabs(nb) - p->ipart) * g_pow10[prec]);
	rem = p->fpart - ft_trunc(p->fpart);
	if (rem > 0.5 || (rem == 0.5 && size != Size_longdouble))
		p->fpart++;
	if (p->fpart > g_pow10[prec])
	{
		p->ipart++;
		p->fpart = 0;
	}
}

static int				calc_extra_zeros(long double fpart, int prec)
{
	int		i;

	i = 0;
	while (fpart * g_pow10[i] < g_pow10[prec] && i <= prec)
		i++;
	i = i - 1;
	return (i);
}

void					pf_dtoa_quick(t_stream *out, long double nb,
		const t_fmt *fmt)
{
	t_parts	p;
	char	buf[MAXBUF_ITOA];
	char	buf2[MAXBUF_ITOA];
	int		pad_len;

	ft_bzero(&p, sizeof(p));
	p.sign = sign_char(nb < 0, fmt);
	p.dot = (fmt->precision > 0 || fmt->alternative_form) ? '.' : 0;
	part_and_round(&p, nb, fmt->precision, fmt->size);
	p.extra_zeros = calc_extra_zeros(p.fpart, fmt->precision);
	p.i_str = (p.ipart == 0.0) ? "0" : pf_utoa_base(buf, (unsigned long)p.ipart,
													10, 0);
	p.f_str = pf_utoa_base(buf2, (unsigned long)p.fpart, 10, 0);
	pad_len = fmt->min_width - ((p.sign != 0) + ft_strlen(p.i_str)
			+ (p.dot != 0) + p.extra_zeros + ft_strlen(p.f_str));
	if (!fmt->left_justify)
		pf_repeat(fmt->padchar, pad_len, out);
	if (p.sign)
		pf_putc(p.sign, out);
	pf_puts(p.i_str,out);
	if (p.dot)
		pf_putc(p.dot, out);
	pf_repeat('0', p.extra_zeros, out);
	pf_puts(p.f_str, out);
	if (fmt->left_justify)
		pf_repeat(fmt->padchar, pad_len, out);
}
