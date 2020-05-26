#include "libftprintf.h"
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
	printf("(0b");
	print_bits(&a, 64);
	printf(")\n");
}

void	print_ld_bits(long double a)
{
	printf("(0b");
	print_bits2(&a, 80);
	printf(")\n");
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

void	test_dbl(const char *format, double a, const char *a_literal)
{
	printf("checking (\"%s\", %s)\n", format, a_literal);
	printf("expected: \"");
	printf(format, a);
	printf("\"\n  actual: \"");
	fflush(stdout);
	ft_printf(format, a);
	printf("\"\n\n");
}

void	test_ld(long double a)
{
	printf("expected: %Lf\n", a);
	ft_printf("actual  : %Lf\n", a);
}

int		main(void)
{
	double	a;

	/*
	test_dbl(-958.125);
	test_dbl(0.3);
	test_dbl(DBL_MIN);
	test_dbl(0.000001);
	test_dbl(DBL_MIN);
	test_dbl("%f", 1.5, "1.5");
	test_dbl("%.4f", 1.5, "1.5");
	test_dbl("%.4f", 1.532673, "1.532673");
	*/
	test_dbl("%f", 0.000039, "0.000039");
	test_dbl("%.0f", 0.000039, "0.000039");
	test_dbl("%5.1f", 7.3, "7.3");
	/*
	long double	b;
	test_ld(-958.125);
	test_ld(-0.3);
	test_ld(LDBL_MAX);
	*/
	return (0);
}
