#include <stdlib.h>
#include "libftprintf.h"

/*
** default_conv:
** (void)f; and (void)ap; suppress unused parameter warnings
*/

void	default_conv(char **str, t_fmt f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = NULL;
}
