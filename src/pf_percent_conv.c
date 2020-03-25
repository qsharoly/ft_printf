#include "libftprintf.h"

/*
** percent_conv:
** (void)f; and (void)ap; suppress unused parameter warings
*/

void	percent_conv(char **str, t_fmt f, va_list ap)
{
	(void)f;
	(void)ap;
	*str = pf_strclone("%");
}
