/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_get_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/15 07:06:23 by debby             #+#    #+#             */
/*   Updated: 2020/08/06 18:30:04 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

/*
** get_none:
** dont get anything from the va_list.
** we have to return something just to conform to the function type signature.
*/

union u_pfarg	get_none(va_list ap, enum e_size size)
{
	union u_pfarg	arg;

	(void)ap;
	(void)size;
	arg.as_i = 0;
	return arg;
}

/*
** TODO: add wide character support
*/

union u_pfarg	get_char(va_list ap, enum e_size size)
{
	union u_pfarg	arg;

	(void)size;
	arg.as_c = (char)va_arg(ap, int);
	return arg;
}

union u_pfarg	get_string(va_list ap, enum e_size size)
{
	union u_pfarg	arg;

	(void)size;
	arg.as_s = va_arg(ap, char *);
	return arg;
}

union u_pfarg	get_pointer(va_list ap, enum e_size size)
{
	union u_pfarg	arg;

	(void)size;
	arg.as_ptr = va_arg(ap, void *);
	return arg;
}

union u_pfarg	get_signed(va_list ap, enum e_size size)
{
	union u_pfarg	arg;

	if (size == Size_hh)
		arg.as_i = (char)va_arg(ap, int);
	else if (size == Size_h)
		arg.as_i = (short)va_arg(ap, int);
	else if (size == Size_l)
		arg.as_i = va_arg(ap, long);
	else if (size == Size_ll)
		arg.as_i = va_arg(ap, long long);
	else
		arg.as_i = va_arg(ap, int);
	return arg;
}

union u_pfarg	get_unsigned(va_list ap, enum e_size size)
{
	union u_pfarg	arg;

	if (size == Size_hh)
		arg.as_u = (unsigned char)va_arg(ap, unsigned int);
	else if (size == Size_h)
		arg.as_u = (unsigned short)va_arg(ap, unsigned int);
	else if (size == Size_l)
		arg.as_u = va_arg(ap, unsigned long);
	else if (size == Size_ll)
		arg.as_u = va_arg(ap, unsigned long long);
	else
		arg.as_u = va_arg(ap, unsigned int);
	return arg;
}

union u_pfarg	get_floating(va_list ap, enum e_size size)
{
	union u_pfarg	arg;

	if (size == Size_longdouble)
		arg.as_ld = va_arg(ap, long double);
	else
		arg.as_d = va_arg(ap, double);
	return arg;
}
