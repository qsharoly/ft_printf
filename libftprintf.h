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
#include <unistd.h>
#include <stdlib.h>

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

int		ft_strlen(char *s);
char	*ft_strclone(const char *src);
int		ft_simple_atoi(const char *s);
char	*ft_itoa_base_abs(long long int value, int base, int min_digits, int upcase);
char	*ft_utoa_base(unsigned long long int value, unsigned int base, int min_digits, int upcase);
void	modulo_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	s_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	c_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	p_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	signed_to_string(char **str, char **prefix, t_fmt fmt, va_list ap);
void	unsigned_to_string(char **str, char **prefix, t_fmt fmt, va_list ap);
void	octal_to_string(char **str, char **prefix, t_fmt fmt, va_list ap);
void	hex_to_string(char **str, char **prefix, t_fmt fmt, va_list ap);
void	default_to_string(char **str, char **prefix, t_fmt f, va_list ap);
int		ft_printf(const char * format, ...);

#endif
