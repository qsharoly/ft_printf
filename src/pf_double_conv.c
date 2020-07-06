/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_double_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 05:31:25 by qsharoly          #+#    #+#             */
/*   Updated: 2020/06/18 21:54:06 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libftprintf.h"
#include "float.h"
#include <limits.h>

void		conv_floating(t_stream *out, t_fmt *fmt, union u_pfarg arg)
{
	if (fmt->has_precision == 0)
		fmt->precision = DTOA_DEFAULT_PRECISION;
	if (fmt->size == Size_longdouble)
	{
		if (ft_fabsl(arg.as_ld) < (long double)ULONG_MAX && fmt->precision < 20)
			pf_put_longdbl_quick(out, arg.as_ld, fmt);
		else
			pf_ldtoa(out, arg.as_ld, fmt);
	}
	else
	{
		if (ft_fabs(arg.as_d) < (double)ULONG_MAX && fmt->precision < 20)
			pf_putdbl_quick(out, arg.as_d, fmt);
		else
			pf_dtoa(out, arg.as_d, fmt);
	}
}
