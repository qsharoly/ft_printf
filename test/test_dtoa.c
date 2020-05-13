#include "../includes/libftprintf.h"
#include <stdio.h>
#include <float.h>
#include <strings.h>

void	print_bits(void *a, int n_bits)
{
	int		i;

	i = n_bits - 1;
	while (i >= 0)
	{
		printf("%d", (*((unsigned long *)a) & (1L << i)) != 0);
		i--;
	}
}

void	print_bits2(void *a, int n_bits)
{
	int		i;
	int		bit;
	char	buf[1005];

	bzero(buf, sizeof(buf));
	i = 0;
	while (i < n_bits)
	{
		bit = (*((unsigned long *)a + i / 64) & (1L << (i % 64))) != 0;
		buf[sizeof(buf) - i - 1] = '0' + bit;
		i++;
	}
	printf("%s", buf + sizeof(buf) - i);
}

void	print_double_bits(double a)
{
	printf("% 10f = \n", a);
	print_bits(&a, 64);
	printf("\n");
}

void	print_ld_bits(long double a)
{
	printf("%Lf = \n", a);
	print_bits2(&a, 80);
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

	/*
	a = -958.125;
	print_double_bits(a);
	printf("libc: %f\n", a);
	ft_printf("you : %f\n", a);
	a = 0.3;
	print_double_bits(a);
	printf("libc: %f\n", a);
	ft_printf("you : %f\n", a);
	a = DBL_MIN;
	print_double_bits(a);
	printf("expected: %f\n", a);
	ft_printf("  actual: %f\n", a);
	*/

	long double	b;
	b = -958.125;
	print_ld_bits(b);
	printf("expected: %Lf\n", b);
	ft_printf("  actual: %Lf\n", b);
	b = -0.3;
	print_ld_bits(b);
	printf("expected: %Lf\n", b);
	ft_printf("  actual: %Lf\n", b);
	/*
	printf("ldblmax = %Lf\n", LDBL_MAX);
	print_ld_bits(LDBL_MAX);
	ft_printf("lets try this. %Lf\n", LDBL_MAX);
	*/
	return (0);
}
