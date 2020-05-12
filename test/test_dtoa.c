#include "../includes/libftprintf.h"
#include <stdio.h>

int		bit_is_set(unsigned long nb, int pos)
{
	return ((nb & (1L << pos)) != 0);
}

void	print_bits(void *a, int n_bits)
{
	int		i;

	i = n_bits - 1;
	while (i >= 0)
	{
		printf("%i", bit_is_set(*((unsigned long *)a), i));
		i--;
	}
}

void	print_double_bytes(double a)
{
	int		i;

	printf("% f = ", a);
	i = 0;
	while (i < (int)sizeof(double))
	{
		printf("%02x ", ((unsigned char *)&a)[i]);
		i++;
	}
	printf("\n");
}


void	print_double_bits(double a)
{
	printf("% 10f = ", a);
	print_bits(&a, 64);
	printf("\n");
}

void	print_char_bits(char c)
{
	print_bits(&c, 8);
	printf("\n");
}

double	bits2double(char *bit_values)
{
	int		i;
	long	nb;

	nb = 0;
	i = 0;
	while (i < 64)
	{
		nb += (1 << i) * (bit_values[64 - i] == '1');
		i++;
	}
	return (*((double *)&(nb)));
}

int		main(void)
{
	double	a;

	a = 0.3;
	print_double_bits(a);
	printf("expected: %f\n", a);
	ft_printf("  actual: %f\n", a);
	return (0);
}
