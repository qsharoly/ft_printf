/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_basic_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:23:11 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/17 11:51:09 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libftprintf.h"
#include "libft.h"

void	default_conv(t_buffer *buf, const t_fmt *f, va_list ap)
{
	(void)buf;
	(void)f;
	(void)ap;
}

void	percent_conv(t_buffer *out, const t_fmt *f, va_list ap)
{
	int		pad_len;

	(void)ap;
	pad_len = ft_imax(0, f->min_width - 1);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_putc('%', out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
}

void	c_conv(t_buffer *out, const t_fmt *f, va_list ap)
{
	int		c;
	int		pad_len;

	(void)f;
	c = va_arg(ap, int);
	pad_len = ft_imax(0, f->min_width - 1);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_putc(c, out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
}

void	s_conv(t_buffer *out, const t_fmt *f, va_list ap)
{
	char	*s;
	int		value_len;
	int		pad_len;

	s = va_arg(ap, char *);
	if (!s)
	{
		s = "(null)";
		value_len = ft_strlen(s);
	}
	else
	{
		if (f->has_precision)
			value_len = ft_imin(ft_strlen(s), f->precision);
		else
			value_len = ft_strlen(s);
	}
	pad_len = ft_imax(0, f->min_width - value_len);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_nputs(s, value_len, out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
}

