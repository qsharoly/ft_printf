#include "libftprintf.h"

/*
** modulo_to_string:
** (void)f; and (void)ap; suppress unused parameter warings
*/

void	modulo_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = ft_strclone("%");
	*prefix = ft_strclone("");
}
