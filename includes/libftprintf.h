/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 15:31:58 by qsharoly          #+#    #+#             */
/*   Updated: 2021/02/28 11:13:31 by debby            ###   ########.fr       */
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
	Size_hh,
	Size_h,
	Size_normal,
	Size_l,
	Size_ll,
	Size_longdouble,
};


typedef struct	s_fmt
{
	unsigned	pad_with_zero:1;
	unsigned	left_justify:1;
	unsigned	explicit_plus:1;
	unsigned	prepend_space:1;
	unsigned	alternative_form:1;
	unsigned	has_precision:1;
	unsigned	upcase:1;
	int			spec_length;
	int			min_width;
	int			precision;
	enum e_size size;
	char		padchar;
	void		(*write_arg)(t_stream *b, struct s_fmt *fmt, va_list ap);
}				t_fmt;

void			(*g_conv_table[256])(t_stream *, t_fmt *, va_list);

typedef struct	s_parts
{
	long double	ipart;
	long double	fpart;
	char		sign;
	char		*i_str;
	char		dot;
	int			extra_zeros;
	char		*f_str;
}				t_parts;

void			init_conv_table(void);
void			putc_printf_internal(int c, t_stream *b);
void			putc_snprintf_internal(int c, t_stream *b);
void			pf_error(const char *msg);
t_stream		pf_stream_init(int target_fd, char *data, int size,
					void (*putc)(int, t_stream*));
void			pf_stream_flush(t_stream *b);
void			pf_putc(int c, t_stream *b);
void			pf_repeat(char c, int times, t_stream *b);
void			pf_puts(const char *s, t_stream *b);
void			pf_nputs(const char *s, int len, t_stream *b);
t_fmt			pf_parse_specifier(const char *str, va_list ap);
char			*pf_utoa_base(char *buffer, unsigned long long value,
					unsigned base, int upcase);
void			pf_putnbr(t_stream *out, const char *value_start,
					const char *prefix, const t_fmt *fmt);
char			sign_char(int is_negative, const t_fmt *fmt);
void			pf_dtoa(t_stream *out, long double d, const t_fmt *fmt);
void			pf_dtoa_quick(t_stream *out, long double nb, const t_fmt *fmt);
void			conv_percent(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_char(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_str(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_ptr(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_signed(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_unsigned(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_oct(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_hex(t_stream *out, t_fmt *fmt, va_list ap);
void			conv_floating(t_stream *out, t_fmt *fmt, va_list ap);
int				ft_printf(const char *format, ...);

#endif
