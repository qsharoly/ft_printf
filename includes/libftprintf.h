/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:31:58 by qsharoly          #+#    #+#             */
/*   Updated: 2022/03/25 23:19:46 by debby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>
# include "sv.h"

# define STDOUT_FD 1
# define BUFFER_SIZE 4096

/*
** enough for binary + terminating '\0'
*/

# define MAXBUF_UTOA (sizeof(long long unsigned) * 8 + 1)
# define DTOA_DEFAULT_PRECISION 6

typedef struct	s_stream
{
	void	(*putc)(int c, struct s_stream *out);
	char	*data;
	int		size;
	int		fd;
	int		total_written;
	int		pos;
	int		space_left;
}				t_stream;

enum			e_size
{
	Size_normal,
	Size_hh,
	Size_h,
	Size_l,
	Size_ll,
	Size_longdouble,
};

enum			e_align
{
	AlignRight,
	AlignLeft,
};

enum			e_plus
{
	OmitSign,
	ExplicitPlus,
	ExplicitSpace,
};

typedef struct	s_fmt
{
	unsigned		align_right_by_leading_zeros:1;
	enum e_align	align:1;
	enum e_plus		plus_mode:2;
	unsigned		alternative_form:1;
	unsigned		has_precision:1;
	unsigned		upcase:1;
	enum e_size 	size;
	int				base;
	int				spec_length;
	int				min_width;
	int				precision;
	void			(*write_arg)(t_stream *b, struct s_fmt *fmt, va_list ap);
}				t_fmt;

typedef struct	s_parts
{
	long double	ipart;
	long double	fpart;
	t_sv		sign;
	t_sv		i_str;
	t_sv		dot;
	int			extra_zeros;
	t_sv		f_str;
}				t_parts;

int				ft_printf(const char *format, ...);
int				ft_snprintf(char *buf, int max, const char *format, ...);

void			pf_error(const char *msg);

t_stream		pf_stream_init(int target_fd, char *data, int size,
					void (*putc)(int, t_stream*));
void			pf_stream_flush(t_stream *b);
void			pf_putc(int c, t_stream *b);
void			pf_puts(const char *s, t_stream *b);
void			put_repeat(char c, int times, t_stream *b);
void			put_sv(t_sv view, t_stream *b);
void			put_sv_padded(t_sv view, int pad_len, enum e_align align,
					t_stream *b);

t_fmt			pf_parse_specifier(const char *str, va_list ap);
t_sv			sign_prefix(int is_negative, const t_fmt *fmt);
t_sv			pf_utoa_base(char *buffer, unsigned long long value,
					unsigned base, int upcase);
void			pf_putnbr(t_stream *out, t_sv value, t_sv prefix,
					const t_fmt *fmt);
void			pf_dtoa(t_stream *out, long double d, const t_fmt *fmt);
void			pf_dtoa_quick(t_stream *out, long double nb, const t_fmt *fmt);

extern void		(*g_conv_table[256])(t_stream *, t_fmt *, va_list);
void			init_conv_table(void);
void			conv_percent(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_char(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_str(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_ptr(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_signed(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_unsigned(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_floating(t_stream *out, t_fmt *fmt, va_list ap);

#endif
