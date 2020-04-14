/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qsharoly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 18:24:37 by qsharoly          #+#    #+#             */
/*   Updated: 2020/04/14 14:34:56 by qsharoly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "libftprintf.h"

void			pf_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

static void		lst_try_push(t_list **list, void const *data, size_t size)
{
	t_list	*elem;

	elem = ft_lstnew(data, size);
	if (elem == NULL)
		pf_error("ft_lstnew() failed");
	ft_lst_push_tail(list, elem);
}

static t_list	*split_convert(const char *format, va_list ap)
{
	t_list		*parts;
	t_fatstr	s;
	t_fmt		fmt;
	char		*cur;

	parts = NULL;
	cur = (char *)format;
	while (*cur)
	{
		if (*cur == '%')
		{
			lst_try_push(&parts, format, cur - format);
			fmt = pf_parse_specifier(cur);
			s = pf_arg_to_string(&fmt, ap);
			lst_try_push(&parts, s.data, s.len);
			free(s.data);
			cur += fmt.spec_length;
			format = cur;
		}
		else
			cur++;
	}
	lst_try_push(&parts, format, cur - format);
	return (parts);
}

static t_fatstr	concat(t_list *parts)
{
	t_list		*tmp;
	t_fatstr	whole;
	int			pos;

	tmp = parts;
	whole.len = 0;
	while (tmp)
	{
		whole.len += tmp->content_size;
		tmp = tmp->next;
	}
	whole.data = malloc(sizeof(*whole.data) * (whole.len + 1));
	if (whole.data == NULL)
		pf_error("malloc() failed");
	whole.data[whole.len] = '\0';
	pos = 0;
	while (parts)
	{
		ft_memcpy(whole.data + pos, parts->content, parts->content_size);
		pos += parts->content_size;
		parts = parts->next;
	}
	return (whole);
}

/*
** size is unused
*/

int				ft_printf(const char *format, ...)
{
	va_list		ap;
	t_list		*parts;
	t_fatstr	s;
	int			total;

	va_start(ap, format);
	parts = split_convert(format, ap);
	va_end(ap);
	s = concat(parts);
	ft_lstdel(&parts, ft_simple_del);
	total = write(1, s.data, s.len);
	free(s.data);
	if (total < 0)
		pf_error("write error");
	return (total);
}
