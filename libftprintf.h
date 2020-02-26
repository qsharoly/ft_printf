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

# define TYPE_MISSING '\0'
# define PAD_WITH_ZEROS 1
# define PAD_FROM_RIGHT 2
# define SPACE_POSITIVE 4
# define PLUS_POSITIVE 8
# define ALTERNATE_FORM 16

typedef			struct s_fmt
{
	int		specifier_length;
	int		min_field_width;
	int		precision;
	long long int	(*get_cast_arg)(va_list ap);
	char	flags;
	char	type;
}				t_fmt;

int		ft_strlen(char *s);
void	ft_strupper(char *s);
int		ft_simple_atoi(const char *s);
char	*ft_itoa_base_abs(long long int value, int base, int min_digits);
char	*ft_itoa_base_unsigned(unsigned long long int value, unsigned int base, int min_digits);
int		ft_printf(const char * format, ...);

#endif
