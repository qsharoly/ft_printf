/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debby <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 11:52:21 by debby             #+#    #+#             */
/*   Updated: 2020/08/06 20:16:46 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLOAT_H
# define FLOAT_H

# define F64_BIAS 1023
# define F80_BIAS 16383

union					u_f64
{
	double				d;
	struct {
		unsigned long	mantissa:52;
		unsigned int	exponent:11;
		unsigned int	sign:1;
	}					bits;
};

union					u_f80
{
	long double			d;
	struct {
		unsigned long	mantissa:64;
		unsigned int	exponent:15;
		unsigned int	sign:1;
	}					bits;
};

int						ft_isinf(double d);
int						ft_isinfl(long double d);
int						ft_isnan(double d);
int						ft_isnanl(long double d);
int						ft_issub(double d);
int						ft_issubl(long double d);
double					ft_fabs(double d);
long double				ft_fabsl(long double d);
double					ft_trunc(double d);
long double				ft_truncl(long double d);

#endif
