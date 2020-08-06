/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_ldtoa_quick.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 04:49:33 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 22:16:33 by qsharoly         ###   ########.fr       */
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

static void				repeat(char c, int times, t_stream *out)
{
	while (times-- > 0)
		pf_putc(c, out);
}

static char				*make_sign(long double nb, const t_fmt *fmt)
{
	if (nb < 0)
		return (ft_strdup("-"));
	else if (fmt->prepend_plus)
		return (ft_strdup("+"));
	else if (fmt->prepend_space)
		return (ft_strdup(" "));
	else
		return (NULL);
}

static void				part_and_round(t_parts *p, long double nb, int prec)
{
	long double	rem;

	p->ipart = ft_truncl(ft_fabsl(nb));
	p->fpart = ((ft_fabsl(nb) - p->ipart) * g_pow10[prec]);
	rem = p->fpart - ft_truncl(p->fpart);
	if ((nb > 0 && rem > 0.5) || (nb < 0 && rem > 0.5))
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

void					pf_ldtoa_quick(t_stream *out, long double nb,
		const t_fmt *fmt)
{
	t_parts	p;
	char	buf[MAXBUF_ITOA];
	char	buf2[MAXBUF_ITOA];
	int		pad_len;
	int		extra_zeros;

	ft_bzero(&p, sizeof(p));
	p.sign = make_sign(nb, fmt);
	part_and_round(&p, nb, fmt->precision);
	extra_zeros = calc_extra_zeros(p.fpart, fmt->precision);
	p.dot = (fmt->precision > 0 || fmt->alternative_form) ? "." : NULL;
	p.i_str = (p.ipart == 0.0) ? "0" : pf_utoa_base(buf, (unsigned long)p.ipart,
													10, 0);
	p.f_str = pf_utoa_base(buf2, (unsigned long)p.fpart, 10, 0);
	pad_len = fmt->min_width - ((p.sign != NULL) + ft_strlen(p.i_str)
			+ (p.dot != NULL) + extra_zeros + ft_strlen(p.f_str));
	if (!fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
	pf_puts_if(p.sign, out);
	pf_puts(p.i_str, out);
	pf_puts_if(p.dot, out);
	repeat('0', extra_zeros, out);
	pf_puts(p.f_str, out);
	if (fmt->left_justify)
		repeat(fmt->padchar, pad_len, out);
}
