#include <stdlib.h>
#include "libftprintf.h"

/*
** c_conv:
** (void)f; suppresses unused parameter warning
*/

void	c_conv(char **str, char **prefix, t_fmt f, va_list ap)
{
	int		nb;

	(void)f;
	nb = va_arg(ap, int);
	*str = malloc(2);
	if (*str == NULL)
		pf_error("malloc error\n");
	(*str)[0] = (char)nb;
	(*str)[1] = '\0';
	*prefix = NULL;
}

/*
** s_conv:
** (void)f; suppresses unused parameter warning
*/

void	s_conv(char **str, char **prefix, t_fmt f, va_list ap)
{
	(void)f;
	*str = va_arg(ap, char *);
	*prefix = NULL;
}

void	p_conv(char **str, char **prefix, t_fmt f, va_list ap)
{
	unsigned long	adr;

	adr = (unsigned long)va_arg(ap, void *);
	if (f.precision == 0 && adr == 0)
		*str = pf_strclone("");
	else
		*str = pf_utoa_base(adr, 16, f.precision, 0);
	*prefix = pf_strclone("0x");
}
