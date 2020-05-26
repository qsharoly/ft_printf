/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:31:58 by qsharoly          #+#    #+#             */
/*   Updated: 2020/05/26 02:45:49 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>

# define STDOUT_FD 1
# define BUFFER_SIZE 4096
# define MAXBUF_ITOA (sizeof(long long int) * 8) //enough for binary
# define TYPE_MISSING '\0'
# define DTOA_DEFAULT_PRECISION 6
# define F64_BIAS 1023
# define F80_BIAS 16383

union	f64
{
	double	d;
	struct
	{
		unsigned long	mantissa:52;
		unsigned int	exponent:11;
		unsigned int	negative:1;
	}	bits;
};

union	f80
{
	long double	d;
	struct
	{
		unsigned long	mantissa:64;
		unsigned int	exponent:15;
		unsigned int	negative:1;
	} bits;
};

typedef struct		s_buffer
{
	char	data[BUFFER_SIZE];
	int		target_fd;
	int		total_written;
	int		pos;
	int		space_left;
}					t_buffer;

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
	unsigned	is_quad:1;
	unsigned	has_precision:1;
	int			spec_length;
	int			min_width;
	int			value_width;
	int			precision;
	char		type;
	char		padchar;
	void		(*write_arg)(t_buffer *b, const struct s_fmt *fmt, va_list ap);
}				t_fmt;

void		pf_error(const char *msg);
void		pf_putc(int c, t_buffer *buf);
void		pf_puts(char *s, t_buffer *b);
void		pf_nputs(char *s, int len, t_buffer *b);
t_fmt		pf_parse_specifier(const char *str);
int			pf_strchr_idx(char needle, const char *hay);
int			pf_simple_atoi(const char *s);
char		*pf_utoa_base(char *buffer, unsigned long long value,
				unsigned base, int upcase);
void		pf_dtoa(t_buffer *out, double d, int precision, const t_fmt *fmt);
//void		pf_ldtoa(t_buffer *out, long double d, int precision, const t_fmt *fmt);
void		default_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		percent_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		s_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		c_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		p_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		signed_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		unsigned_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		octal_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		hex_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
void		double_conv(t_buffer *out, const t_fmt *fmt, va_list ap);
int			ft_printf(const char * format, ...);

#endif
