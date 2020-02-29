#include "libftprintf.h"

/*
** c_to_string:
** (void)f; suppresses unused parameter warning
*/

void	c_to_string(char **str, char **prefix, t_fmt f, va_list ap)
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
	*prefix = ft_strclone("");
}

/*
** s_to_string:
** (void)f; suppresses unused parameter warning
*/

void	s_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	(void)f;
	*str = va_arg(ap, char *);
	*prefix = ft_strclone("");
}

void	p_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	adr;

	adr = (unsigned long)va_arg(ap, void *);
	if (f.precision == 0 && adr == 0)
		*str = ft_strclone("");
	else
		*str = ft_itoa_base_unsigned(adr, 16, f.precision, 0);
	*prefix = ft_strclone("0x");
}
