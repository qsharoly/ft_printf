/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:31:58 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/26 17:44:08 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

#include <stdarg.h>

# define TYPE_MISSING '\0'

typedef			struct s_fmt
{
	unsigned	pad_with_zero:1;
	unsigned	left_justify:1;
	unsigned	prepend_space:1;
	unsigned	prepend_plus:1;
	unsigned	alternative_form:1;
	unsigned	is_char:1;
	unsigned	is_short:1;
	unsigned	is_int:1;
	unsigned	is_long:1;
	unsigned	is_longlong:1;
	unsigned	has_precision:1;
	int			spec_length;
	int			min_field_width;
	int			precision;
	char		type;
	void		(*to_string)(char **str, char **prefix, struct s_fmt f, va_list ap);
}				t_fmt;

void	pf_error(const char *msg);
char	*pf_strclone(const char *src);
int		pf_simple_atoi(const char *s);
char	*pf_itoa_base_abs(long long int value, int base, int min_digits, int upcase);
char	*pf_utoa_base(unsigned long long int value, unsigned int base, int min_digits, int upcase);
void	percent_conv(char **str, char **prefix, t_fmt f, va_list ap);
void	s_conv(char **str, char **prefix, t_fmt f, va_list ap);
void	c_conv(char **str, char **prefix, t_fmt f, va_list ap);
void	p_conv(char **str, char **prefix, t_fmt f, va_list ap);
void	signed_conv(char **str, char **prefix, t_fmt fmt, va_list ap);
void	unsigned_conv(char **str, char **prefix, t_fmt fmt, va_list ap);
void	octal_conv(char **str, char **prefix, t_fmt fmt, va_list ap);
void	hex_conv(char **str, char **prefix, t_fmt fmt, va_list ap);
void	default_conv(char **str, char **prefix, t_fmt f, va_list ap);
int		ft_printf(const char * format, ...);

#endif
