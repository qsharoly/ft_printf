#include <stdio.h>

int		main(void)
{
	double	d;
	int		i;

	d = 0.3 * (1 << 20);
	i = 0;
	while (i < 100)
	{
		printf("%f\n", d);
		d *= 2;
		i++;
	}
	return (0);
}
