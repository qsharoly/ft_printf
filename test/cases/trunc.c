#include "../includes/float.h"
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

int	check_double(double d)
{
	double	actual = ft_trunc(d);
	double	expect = trunc(d);

	if (actual != expect)
	{
		printf("failed at %f:\n", d);
		printf("ft_trunc -> %f\n   trunc -> %f\n",
				actual, expect);
	}
	return (actual == expect);
}

int	check_long_double(long double d)
{
	long double	actual = ft_trunc(d);
	long double	expect = trunc(d);

	if (actual != expect)
	{
		printf("failed at %Lf:\n", d);
		printf("ft_trunc -> %Lf\n   trunc -> %Lf\n",
				actual, expect);
	}
	return (actual == expect);
}

int	main(int ac, char **av)
{
	printf("\n=== Floats: trunc ===\n");
	printf("checking double\n");
	double	d = (double)ULONG_MAX * 100;
	while (fabs(d *= -0.71) > 0.0001 && check_double(d));
	printf("checking long double\n");
	long double	ld = (double)ULONG_MAX * 100;
	while (fabs(ld *= -0.71) > 0.0001 && check_long_double(ld));
	return (21);
}

