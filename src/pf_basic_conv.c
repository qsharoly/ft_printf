/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_basic_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:23:11 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 19:48:08 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libftprintf.h"
#include "libft.h"

void	default_conv(t_buffer *out, t_fmt *f, va_list ap)
{
	(void)out;
	(void)f;
	(void)ap;
}

void	percent_conv(t_buffer *out, t_fmt *f, va_list ap)
{
#if __APPLE__
	int		pad_len;

	(void)ap;
	pad_len = ft_imax(0, f->min_width - 1);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_putc('%', out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
#elif __linux__
	(void)ap;
	(void)f;
	pf_putc('%', out);
#endif
}

void	c_conv(t_buffer *out, t_fmt *f, va_list ap)
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

void	s_conv(t_buffer *out, t_fmt *f, va_list ap)
{
	char	*s;
	int		value_len;
	int		pad_len;

	s = va_arg(ap, char *);
	if (!s)
		s = "(null)";
	if (f->has_precision && f->precision >= 0)
		value_len = ft_imin(ft_strlen(s), f->precision);
	else
		value_len = ft_strlen(s);
	pad_len = ft_imax(0, f->min_width - value_len);
	while (!f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
	pf_nputs(s, value_len, out);
	while (f->left_justify && pad_len-- > 0)
		pf_putc(f->padchar, out);
}
