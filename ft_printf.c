/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/02/28 20:53:49 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libftprintf.h"

static int	flag_is_set(const char flags, const char which)
{
	return ((flags & which) != 0);
}

static int	char_in_str(char needle, const char *hay)
{
	while (*hay)
	{
		if (*hay == needle)
			return (1);
		hay++;
	}
	return (0);
}

#define HH 0
#define H 1
#define REGULAR 2
#define L 3
#define LL 4
#define SIGNED 0
#define UNSIGNED 1
#define OCTAL 2
#define HEX 3

static char	*ft_strdup(const char *src)
{
	char	*str;
	int		len;
	int		i;

	len = ft_strlen((char *)src);
	/*
	if (len == INT_MAX)
		return (NULL);
		*/
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = src[i];
		i++;
	}
	str[len] = '\0';
	return (str);
}

/*
** percent_sign_to_string:
** (void)f; and (void)ap; suppress unused parameter warings
*/

static void	percent_sign_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = ft_strdup("%");
	*prefix = ft_strdup("");
}

/*
** s_to_string:
** (void)f; suppresses unused parameter warning
*/

static void	s_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	(void)f;
	*str = va_arg(ap, char *);
	*prefix = ft_strdup("");
}

/*
** c_to_string:
** (void)f; suppresses unused parameter warning
*/

static void	c_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	int		nb;

	(void)f;
	nb = va_arg(ap, int);
	*str = malloc(2);
	if (*str == NULL)
	{
		write(2, "failed malloc\n", 14);
		exit(1);
	}
	(*str)[0] = (char)nb;
	(*str)[1] = '\0';
	*prefix = ft_strdup("");
}

static void	p_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	adr;

	adr = (unsigned long)va_arg(ap, void *);
	if (f.precision == 0 && adr == 0)
		*str = ft_strdup("");
	else
		*str = ft_itoa_base_unsigned(adr, 16, f.precision, "0123456789abcdef");
	*prefix = ft_strdup("0x");
}

static void signed_to_string(char **str, char **prefix, t_fmt fmt, long long int nb)
{
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strdup("");
	else
		*str = ft_itoa_base_abs(nb, 10, fmt.precision, "0123456789");
	if (nb >= 0)
	{
		if (flag_is_set(fmt.flags, PLUS_POSITIVE))
			*prefix = ft_strdup("+");
		else if (flag_is_set(fmt.flags, SPACE_POSITIVE))
			*prefix = ft_strdup(" ");
		else
			*prefix = ft_strdup("");
	}
	else
		*prefix = ft_strdup("-");
}

static void hhd(char **str, char **prefix, t_fmt f, va_list ap)
{
	char	nb;

	nb = (char)va_arg(ap, int);
	signed_to_string(str, prefix, f, nb);
}

static void hd(char **str, char **prefix, t_fmt f, va_list ap)
{
	short	nb;

	nb = (short)va_arg(ap, int);
	signed_to_string(str, prefix, f, nb);
}

static void d(char **str, char **prefix, t_fmt f, va_list ap)
{
	int		nb;

	nb = va_arg(ap, int);
	signed_to_string(str, prefix, f, nb);
}
static void ld(char **str, char **prefix, t_fmt f, va_list ap)
{
	long	nb;

	nb = va_arg(ap, long);
	signed_to_string(str, prefix, f, nb);
}

static void	lld(char **str, char **prefix, t_fmt f, va_list ap)
{
	long long	nb;

	nb = va_arg(ap, long long);
	signed_to_string(str, prefix, f, nb);
}

static void unsigned_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb)
{
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strdup("");
	else
		*str = ft_itoa_base_unsigned(nb, 10, fmt.precision, "0123456789");
	*prefix = ft_strdup("");
}

static void hhu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned char	nb;

	nb = (unsigned char)va_arg(ap, unsigned int);
	unsigned_to_string(str, prefix, f, nb);
}

static void hu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned short	nb;

	nb = (unsigned short)va_arg(ap, unsigned int);
	unsigned_to_string(str, prefix, f, nb);
}

static void u(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned int	nb;

	nb = va_arg(ap, unsigned int);
	unsigned_to_string(str, prefix, f, nb);
}

static void lu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	nb;

	nb = va_arg(ap, unsigned long);
	unsigned_to_string(str, prefix, f, nb);
}

static void llu(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long long	nb;

	nb = va_arg(ap, unsigned long long);
	unsigned_to_string(str, prefix, f, nb);
}

static void	octal_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb)
{
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strdup("");
	else
		*str = ft_itoa_base_unsigned(nb, 8, fmt.precision, "01234567");
	if (flag_is_set(fmt.flags, ALTERNATE_FORM) && ((nb != 0 && fmt.precision <= 0) || (nb == 0 && fmt.precision == 0)))
		*prefix = ft_strdup("0");
	else
		*prefix = ft_strdup("");
}

static void	hho(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned char	nb;

	nb = (unsigned char)va_arg(ap, unsigned int);
	octal_to_string(str, prefix, f, nb);
}

static void	ho(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned short	nb;

	nb = (unsigned short)va_arg(ap, unsigned int);
	octal_to_string(str, prefix, f, nb);
}

static void	o(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned int	nb;

	nb = va_arg(ap, unsigned int);
	octal_to_string(str, prefix, f, nb);
}

static void	lo(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	nb;

	nb = va_arg(ap, unsigned long);
	octal_to_string(str, prefix, f, nb);
}

static void	llo(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long long	nb;

	nb = va_arg(ap, unsigned long long);
	octal_to_string(str, prefix, f, nb);
}

static void hex_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb)
{
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strdup("");
	else if (fmt.type == 'x')
		*str = ft_itoa_base_unsigned(nb, 16, fmt.precision, "0123456789abcdef");
	else
		*str = ft_itoa_base_unsigned(nb, 16, fmt.precision, "0123456789ABCDEF");
	if (flag_is_set(fmt.flags, ALTERNATE_FORM) && nb != 0)
		*prefix = fmt.type == 'x' ? ft_strdup("0x") : ft_strdup("0X");
	else
		*prefix = ft_strdup("");
}

static void hhxX(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned char	nb;

	nb = (unsigned char)va_arg(ap, unsigned int);
	hex_to_string(str, prefix, f, nb);
}

static void hxX(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned short	nb;

	nb = (unsigned short)va_arg(ap, unsigned int);
	hex_to_string(str, prefix, f, nb);
}

static void xX(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned int	nb;

	nb = va_arg(ap, unsigned int);
	hex_to_string(str, prefix, f, nb);
}

static void lxX(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	nb;

	nb = va_arg(ap, unsigned long);
	hex_to_string(str, prefix, f, nb);
}

static void llxX(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long long	nb;

	nb = va_arg(ap, unsigned long long);
	hex_to_string(str, prefix, f, nb);
}

/*
** default_to_string:
** (void)f; and (void)ap; suppress unused parameter warnings
*/

static void	default_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = NULL;
	*prefix = ft_strdup("");
}

void (*g_integer_to_string[4][5])(char **, char **, t_fmt, va_list) = {
	{hhd, hd, d, ld, lld},
	{hhu, hu, u, lu, llu},
	{hho, ho, o, lo, llo},
	{hhxX, hxX, xX, lxX, llxX}
};

void	(*choose_to_string_func(int size, char type))(char **, char **, t_fmt, va_list)
{
	if (type == '%')
		return (percent_sign_to_string);
	else if (type == 'c')
		return (c_to_string);
	else if (type == 's')
		return (s_to_string);
	else if (type == 'p')
		return (p_to_string);
	else if (type == 'd' || type == 'i')
		return (g_integer_to_string[SIGNED][size]);
	else if (type == 'u')
		return (g_integer_to_string[UNSIGNED][size]);
	else if (type == 'o')
		return (g_integer_to_string[OCTAL][size]);
	else if (type == 'x' || type == 'X')
		return (g_integer_to_string[HEX][size]);
	else
		return (default_to_string);
}

/*
** repeated flag symbols are skipped
*/

static t_fmt	get_format(const char *str)
{
	t_fmt	fmt;
	char	*ptr;
	int		size;

	ptr = (char *)str + 1;
	fmt.min_field_width = 0;
	fmt.precision = -1;
	fmt.flags = 0;
	while (char_in_str(*ptr, "0-+ #"))
	{
		if (*ptr == '0')
			fmt.flags |= PAD_WITH_ZEROS;
		else if (*ptr == '-')
			fmt.flags |= PAD_FROM_RIGHT;
		else if (*ptr == '+')
			fmt.flags |= PLUS_POSITIVE;
		else if (*ptr == ' ')
			fmt.flags |= SPACE_POSITIVE;
		else if (*ptr == '#')
			fmt.flags |= ALTERNATE_FORM;
		ptr++;
	}
	fmt.min_field_width = ft_simple_atoi(ptr);
	while (*ptr >= '0' && *ptr <= '9')
		ptr++;
	if (*ptr == '.')
	{
		ptr++;
		fmt.precision = ft_simple_atoi(ptr);
		while (*ptr >= '0' && *ptr <= '9')
			ptr++;
	}
	if (*ptr == 'h' && *(ptr + 1) == 'h')
	{
		ptr += 2;
		size = HH;
	}
	else if (*ptr == 'h')
	{
		ptr++;
		size = H;
	}
	else if (*ptr == 'l' && *(ptr + 1) == 'l')
	{
		ptr += 2;
		size = LL;
	}
	else if (*ptr == 'l')
	{
		ptr++;
		size = L;
	}
	else
		size = REGULAR;
	if (char_in_str(*ptr, "%cspdiuoxX"))
	{
		fmt.type = *ptr;
		ptr++;
	}
	else
		fmt.type = TYPE_MISSING;
	fmt.to_string = choose_to_string_func(size, fmt.type);
	fmt.specifier_length = ptr - str;
	return (fmt);
}

static char *make_pad(int padlen, t_fmt fmt)
{
	int		i;
	char	*pad;
	char	padchar;

	if (flag_is_set(fmt.flags, PAD_WITH_ZEROS) && !flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		padchar = '0';
	else
		padchar = ' ';
	if (flag_is_set(fmt.flags, PAD_WITH_ZEROS) && (fmt.type == 'd' || fmt.type == 'i') && fmt.precision != -1)
		padchar = ' ';
	pad = malloc(padlen + 1);
	if (pad == NULL)
	{
		write(2, "failed malloc", 13);
		exit(1);
	}
	i = 0;
	while (i < padlen)
	{
		pad[i] = padchar;
		i++;
	}
	pad[padlen] = '\0';
	return (pad);
}

static void	put(t_fmt fmt, va_list ap, int *total)
{
	char	*str;
	char	*pad;
	char	*prefix;
	int		padlen;
	int		strlen;

	str = NULL;
	prefix = NULL;
	fmt.to_string(&str, &prefix, fmt, ap);
	if (str != NULL)
	{
		if (fmt.type == 'c')
			strlen = 1;
		else if (fmt.type == 's' && fmt.precision != -1 && fmt.precision < ft_strlen(str))
			strlen = fmt.precision;
		else
			strlen = ft_strlen(str);
		*total += strlen + ft_strlen(prefix);
		padlen = fmt.min_field_width - strlen - ft_strlen(prefix);
		pad = NULL;
		if (padlen > 0)
		{
			pad = make_pad(padlen, fmt);
			*total += padlen;
		}
		if (pad && !flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		{
			if (flag_is_set(fmt.flags, PAD_WITH_ZEROS) && (fmt.type == 'd' || fmt.type == 'i' || flag_is_set(fmt.flags, ALTERNATE_FORM)))
			{
				write(1, prefix, ft_strlen(prefix));
				write(1, pad, padlen);
			}
			else
			{
				write(1, pad, padlen);
				write(1, prefix, ft_strlen(prefix));
			}
			write(1, str, strlen);
		}
		else if (pad && flag_is_set(fmt.flags, PAD_FROM_RIGHT))
		{
			write(1, prefix, ft_strlen(prefix));
			write(1, str, strlen);
			write(1, pad, padlen);
		}
		else
		{
			write(1, prefix, ft_strlen(prefix));
			write(1, str, strlen);
		}
		free(pad);
	}
	else if (fmt.type == 's')
	{
		*total += 6;
		write(1, "(null)", 6);
	}
	if (fmt.type != 's')
		free(str);
	free(prefix);
}

int				ft_printf(const char * format, ...)
{
	char	*start;
	char	*cur;
	va_list	ap;
	t_fmt	f;
	int		total;

	total = 0;
	start = (char *)format;
	cur = (char *)format;
	va_start(ap, format);
	while (*cur)
	{
		if (*cur == '%')
		{
			write(1, start, cur - start);
			total += cur - start;
			f = get_format(cur);
			put(f, ap, &total);
			cur += f.specifier_length;
			start = cur;
		}
		else
			cur++;
	}
	write(1, start, cur - start);
	total += cur - start;
	va_end(ap);
	return (total);
}
