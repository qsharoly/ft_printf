/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:31:58 by qsharoly          #+#    #+#             */
/*   Updated: 2020/08/09 14:20:25 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>

# define STDOUT_FD 1
# define BUFFER_SIZE 4096

/*
** enough for binary + terminating '\0'
*/

# define MAXBUF_ITOA (sizeof(long long int) * 8 + 1)
# define DTOA_DEFAULT_PRECISION 6

typedef struct	s_stream
{
	char	data[BUFFER_SIZE];
	int		fd;
	int		total_written;
	int		pos;
	int		space_left;
}				t_stream;

union			u_pfarg
{
	long long int		as_i;
	long long unsigned	as_u;
	long double			as_f;
	void				*as_ptr;
	char				*as_s;
	char				as_c;
};

enum			e_size
{
	Size_hh,
	Size_h,
	Size_normal,
	Size_l,
	Size_ll,
	Size_longdouble,
};

enum			e_conv
{
	Conv_percent = 0,
	Conv_caracter,
	Conv_string,
	Conv_pointer,
	Conv_signed_d,
	Conv_signed_i,
	Conv_unsigned,
	Conv_octal,
	Conv_hex,
	Conv_hex_uppercase,
	Conv_floating,
	Conv_none,
};

typedef struct	s_fmt
{
	unsigned	pad_with_zero:1;
	unsigned	left_justify:1;
	unsigned	prepend_space:1;
	unsigned	prepend_plus:1;
	unsigned	alternative_form:1;
	unsigned	has_precision:1;
	int			spec_length;
	int			min_width;
	int			value_width;
	int			precision;
	enum e_size size;
	enum e_conv	conv;
	char		padchar;
	void		(*write_arg)(t_stream *b, struct s_fmt *fmt, va_list ap);
}				t_fmt;

typedef struct	s_parts
{
	double	ipart;
	double	fpart;
	char	sign;
	char	*i_str;
	char	dot;
	int		extra_zeros;
	char	*f_str;
}				t_parts;

void			pf_error(const char *msg);
void			pf_stream_init(t_stream *b, int target_fd);
void			pf_stream_flush(t_stream *b);
void			pf_putc(int c, t_stream *b);
void			pf_repeat(char c, int times, t_stream *b);
void			pf_puts(const char *s, t_stream *b);
void			pf_puts_if(const char *s, t_stream *b);
void			pf_nputs(const char *s, int len, t_stream *b);
t_fmt			pf_specifier_parse(const char *str, va_list ap);
int				pf_strget_index(const char *hay, char needle);
int				pf_simple_atoi(const char *s);
char			*pf_utoa_base(char *buffer, unsigned long long value,
					unsigned base, int upcase);
void			pf_putnbr(t_stream *out, const char *value_start,
					const char *prefix, const t_fmt *fmt);
void			pf_dtoa(t_stream *out, long double d, const t_fmt *fmt);
void			pf_dtoa_quick(t_stream *out, long double nb, const t_fmt *fmt);
void			conv_percent(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_s(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_c(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_p(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_signed(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_unsigned(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_oct(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_hex(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_floating(t_stream *out, t_fmt *fmt, union u_pfarg arg);
void			conv_default(t_stream *out, t_fmt *fmt, union u_pfarg arg);
union u_pfarg	get_none(va_list ap, enum e_size size);
union u_pfarg	get_char(va_list ap, enum e_size size);
union u_pfarg	get_string(va_list ap, enum e_size size);
union u_pfarg	get_pointer(va_list ap, enum e_size size);
union u_pfarg	get_signed(va_list ap, enum e_size size);
union u_pfarg	get_unsigned(va_list ap, enum e_size size);
union u_pfarg	get_floating(va_list ap, enum e_size size);
int				ft_printf(const char *format, ...);

#endif
