/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:31:58 by qsharoly          #+#    #+#             */
/*   Updated: 2020/04/14 14:34:44 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

#include <stdarg.h>

# define TYPE_MISSING '\0'

typedef			struct s_fat
{
	char	*data;
	int		len;
}				t_fatstr;

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
	int			min_width;
	int			precision;
	char		type;
	void		(*to_string)(char **str, const struct s_fmt *fmt, va_list ap);
}				t_fmt;

void		pf_error(const char *msg);
t_fmt		pf_parse_specifier(const char *str);
t_fatstr	pf_arg_to_string(const t_fmt *fmt, va_list ap);
int			pf_max(int a, int b);
char		*pf_strclone(const char *src);
int			pf_strchr_idx(char needle, const char *hay);
int			pf_simple_atoi(const char *s);
char		*pf_itoa_dec(long long value, int min_digits, char positive_prefix);
char		*pf_utoa_base(unsigned long long value, unsigned int base, int min_digits, int upcase);
char		*pf_utoa_oct(unsigned long long value, int min_digits, int prefix);
char		*pf_utoa_hex(unsigned long long value, int min_digits, int prefix, int upper);
void		default_conv(char **str, const t_fmt *fmt, va_list ap);
void		percent_conv(char **str, const t_fmt *fmt, va_list ap);
void		s_conv(char **str, const t_fmt *fmt, va_list ap);
void		c_conv(char **str, const t_fmt *fmt, va_list ap);
void		p_conv(char **str, const t_fmt *fmt, va_list ap);
void		signed_conv(char **str, const t_fmt *fmt, va_list ap);
void		unsigned_conv(char **str, const t_fmt *fmt, va_list ap);
void		octal_conv(char **str, const t_fmt *fmt, va_list ap);
void		hex_conv(char **str, const t_fmt *fmt, va_list ap);
int			ft_printf(const char * format, ...);

#endif
