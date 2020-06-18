#include "libftprintf.h"
#include <stdio.h>

int		main(void)
{
	//wildcard width & precision
	ft_printf("{%*3d}\n", 5, 0);
	printf("{%*3d}\n", 5, 0);
	ft_printf("{%05.*d}\n", -15, 42);
	printf("{%05.*d}\n", -15, 42);
	return (0);
}
