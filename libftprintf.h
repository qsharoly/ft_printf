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

/*
 * flags
 */
# define PAD_WITH_ZEROS 1
# define PAD_FROM_RIGHT 2
# define SPACE_POSITIVE 4
# define PLUS_POSITIVE 8
# define ALTERNATE_FORM 16
/*
 * integer type sizes
 */
# define HH 0
# define H 1
# define REGULAR 2
# define L 3
# define LL 4
/*
 * type conversions
 */
# define SIGNED 0
# define UNSIGNED 1
# define OCTAL 2
# define HEX 3
# define TYPE_MISSING '\0'

typedef			struct s_fmt
{
	int		specifier_length;
	int		min_field_width;
	int		precision;
	void	(*to_string)(char **str, char **prefix, struct s_fmt f, va_list ap);
	char	flags;
	char	type;
}				t_fmt;

int		flag_is_set(char flags, char which);
int		ft_strlen(char *s);
char	*ft_strclone(const char *src);
int		ft_simple_atoi(const char *s);
char	*ft_itoa_base_abs(long long int value, int base, int min_digits, int upcase);
char	*ft_itoa_base_unsigned(unsigned long long int value, unsigned int base, int min_digits, int upcase);
void	modulo_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	s_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	c_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	p_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	signed_to_string(char **str, char **prefix, t_fmt fmt, long long int nb);
void	unsigned_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb);
void	octal_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb);
void	hex_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb);
void	default_to_string(char **str, char **prefix, t_fmt f, va_list ap);
void	hhd(char **str, char **prefix, t_fmt f, va_list ap);
void	hd(char **str, char **prefix, t_fmt f, va_list ap);
void	d(char **str, char **prefix, t_fmt f, va_list ap);
void	ld(char **str, char **prefix, t_fmt f, va_list ap);
void	lld(char **str, char **prefix, t_fmt f, va_list ap);
void	hhu(char **str, char **prefix, t_fmt f, va_list ap);
void	hu(char **str, char **prefix, t_fmt f, va_list ap);
void	u(char **str, char **prefix, t_fmt f, va_list ap);
void	lu(char **str, char **prefix, t_fmt f, va_list ap);
void	llu(char **str, char **prefix, t_fmt f, va_list ap);
void	hho(char **str, char **prefix, t_fmt f, va_list ap);
void	ho(char **str, char **prefix, t_fmt f, va_list ap);
void	o(char **str, char **prefix, t_fmt f, va_list ap);
void	lo(char **str, char **prefix, t_fmt f, va_list ap);
void	llo(char **str, char **prefix, t_fmt f, va_list ap);
void	hhx(char **str, char **prefix, t_fmt f, va_list ap);
void	hx(char **str, char **prefix, t_fmt f, va_list ap);
void	x(char **str, char **prefix, t_fmt f, va_list ap);
void	lx(char **str, char **prefix, t_fmt f, va_list ap);
void	llx(char **str, char **prefix, t_fmt f, va_list ap);
int		ft_printf(const char * format, ...);

#endif
