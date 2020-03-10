#include "libft.h"
#include "libftprintf.h"

char	*pf_strclone(const char *src)
{
	char	*clone;

	clone = ft_strdup(src);
	if (!clone)
		pf_error("malloc error\n");
	return (clone);
}
