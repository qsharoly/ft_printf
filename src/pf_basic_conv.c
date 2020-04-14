/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_basic_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:23:11 by qsharoly          #+#    #+#             */
/*   Updated: 2020/04/14 14:36:46 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libftprintf.h"

void	default_conv(char **str, const t_fmt *f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = NULL;
}

void	percent_conv(char **str, const t_fmt *f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = pf_strclone("%");
}

void	c_conv(char **str, const t_fmt *f, va_list ap)
{
	int		nb;

	(void)f;
	nb = va_arg(ap, int);
	*str = malloc(2);
	if (*str == NULL)
		pf_error("malloc error\n");
	(*str)[0] = (char)nb;
	(*str)[1] = '\0';
}

void	s_conv(char **str, const t_fmt *f, va_list ap)
{
	(void)f;
	*str = va_arg(ap, char *);
}

void	p_conv(char **str, const t_fmt *f, va_list ap)
{
	unsigned long	adr;
	int				need_prefix;
	int				upcase;

	adr = (unsigned long)va_arg(ap, void *);
	need_prefix = 1;
	upcase = 0;
	if (f->precision == 1 && adr == 0)
		*str = pf_strclone("0x0");
	else
		*str = pf_utoa_hex(adr, f->precision, need_prefix, upcase);
}
