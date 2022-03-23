/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_conv_diuox.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:55:31 by qsharoly          #+#    #+#             */
/*   Updated: 2022/03/24 01:40:55 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

#if __linux__

void	conv_ptr(t_stream *out, t_fmt *fmt, va_list ap)
{
	char	buffer[MAXBUF_UTOA];
	t_sv	value;
	void	*p;

	p = va_arg(ap, void *);
	if (!p)
	{
		value = sv_from_cstr("(nil)");
		put_sv_padded(value, fmt->min_width - value.length, fmt->align, out); 
		return ;
	}
	value = pf_utoa_base(buffer, (unsigned long)p, 16, 0);
	pf_putnbr(out, value, sv_from_cstr("0x"), fmt);
}

#else

void	conv_ptr(t_stream *out, t_fmt *fmt, va_list ap)
{
	char	buffer[MAXBUF_UTOA];
	t_sv	value;
	void	*p;

	p = va_arg(ap, void *);
	value = pf_utoa_base(buffer, (unsigned long)p, 16, 0);
	pf_putnbr(out, value, sv_from_cstr("0x"), fmt);
}

#endif

void	conv_signed(t_stream *out, t_fmt *fmt, va_list ap)
{
	char			buffer[MAXBUF_UTOA];
	t_sv			value;
	t_sv			prefix;
	long long int	arg;

	if (fmt->size == Size_ll)
		arg = va_arg(ap, long long);
	else if (fmt->size == Size_l)
		arg = va_arg(ap, long);
	else
		arg = va_arg(ap, int);
	if (fmt->size == Size_hh)
		arg = (char)arg;
	else if (fmt->size == Size_h)
		arg = (short)arg;
	value = pf_utoa_base(buffer, ft_abs(arg), 10, 0);
	prefix = sign_prefix(arg < 0, fmt);
	pf_putnbr(out, value, prefix, fmt);
}

void	conv_unsigned(t_stream *out, t_fmt *fmt, va_list ap)
{
	char				buffer[MAXBUF_UTOA];
	t_sv				value;
	char				*prefix;
	unsigned long long	arg;

	if (fmt->size == Size_ll)
		arg = va_arg(ap, unsigned long long);
	else if (fmt->size == Size_l)
		arg = va_arg(ap, unsigned long);
	else
		arg = va_arg(ap, unsigned int);
	if (fmt->size == Size_hh)
		arg = (unsigned char)arg;
	else if (fmt->size == Size_h)
		arg = (unsigned short)arg;
	value = pf_utoa_base(buffer, arg, fmt->base, fmt->upcase);
	prefix = "";
	if (fmt->alternative_form)
	{
		if (fmt->base == 8 && value.length >= fmt->precision)
			prefix = "0";
		if (fmt->base == 16 && arg > 0)
			prefix = fmt->upcase ? "0X" : "0x";
	}
	pf_putnbr(out, value, sv_from_cstr(prefix), fmt);
}
