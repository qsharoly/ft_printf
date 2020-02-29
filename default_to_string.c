#include "libftprintf.h"

/*
** default_to_string:
** (void)f; and (void)ap; suppress unused parameter warnings
*/

void	default_to_string(char **str, char **prefix, t_fmt f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = NULL;
	*prefix = ft_strclone("");
}
