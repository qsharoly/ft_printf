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

void	check_dbl(const char *format, double a, const char *literal)
{
	printf("checking (\"%s\", %s)\n", format, literal);
	printf("expected: \"");
	printf(format, a);
	printf("\"\n  actual: \"");
	fflush(stdout);
	ft_printf(format, a);
	printf("\"\n\n");
}

void	check_ld(const char *format, long double a, const char *literal)
{
	printf("checking (\"%s\", %s)\n", format, literal);
	printf("expected: \"");
	printf(format, a);
	printf("\"\n  actual: \"");
	fflush(stdout);
	ft_printf(format, a);
	printf("\"\n\n");
}

int		main(void)
{
	check_dbl("%f", -958.125, "-958.125");
	check_dbl("%f", __DBL_MIN__, "DBL_MIN");
	check_dbl("%f", __DBL_MAX__, "DBL_MAX");
	check_dbl("%f", 1.05, "1.05");
	check_dbl("%.4f", 1.05, "1.05");
	check_dbl("%.4f", 1.532673, "1.532673");
	check_dbl("%f", 0.000039, "0.000039");
	check_dbl("%.0f", 0.000039, "0.000039");
	check_dbl("%5.1f", 7.3, "7.3");
	check_dbl("%5.1f", -7.3, "-7.3");
	check_dbl("%-5.0f", -7.3, "-7.3");
	check_dbl("%-5.0f", 7.5, "7.5");
	check_dbl("%-5.0f", -7.5, "-7.5");
	check_dbl("%f", -3.85, "-3.85");
	check_dbl("%f", -0.99999949, "-0.99999949");
	/*
	check_ld("%Lf", -958.125, "-958.125");
	check_ld("%Lf", -0.3, "-0.3");
	check_ld("%Lf", LDBL_MAX, "LDBL_MAX");
	check_ld("%Lf", LDBL_MIN, "LDBL_MIN");
	check_ld("%.0Lf", LDBL_MIN, "LDBL_MIN");
	print_ld_bits(LDBL_MAX);
	print_ld_bits(LDBL_MIN);
	*/
	return (0);
}
