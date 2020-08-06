/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 19:10:22 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/06 19:25:48 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"

void	pf_putnbr(t_stream *out, const char *value_start, const char *prefix,
			const t_fmt *fmt)
{
	int				value_len;
	int				pre_len;
	int				zero_fill;
	int				pad_len;

	value_len = ft_strlen(value_start);
	pre_len = ft_strlen(prefix);
	if (fmt->pad_with_zero && !fmt->left_justify && !fmt->has_precision)
		zero_fill = ft_imax(0, fmt->min_width - value_len - pre_len);
	else
		zero_fill = ft_imax(0, fmt->precision - value_len);
	pad_len = fmt->min_width - pre_len - zero_fill - value_len;
	while (!fmt->left_justify && (pad_len-- > 0))
		pf_putc(fmt->padchar, out);
	if (pre_len)
		pf_puts(prefix, out);
	while (zero_fill-- > 0)
		pf_putc('0', out);
	pf_puts(value_start, out);
	while (fmt->left_justify && (pad_len-- > 0))
		pf_putc(fmt->padchar, out);
}
