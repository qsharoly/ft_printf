#include <stdio.h>
#include <unistd.h>

int		bit_is_set(int nb, int offset)
{
	return ((nb & (1 << offset)) != 0);
}

void	print_float_bytes(float a)
{
	int		i;

	printf("% f = ", a);
	i = 0;
	while (i < (int)sizeof(float))
	{
		printf("%02x ", ((unsigned char *)&a)[i]);
		i++;
	}
	printf("\n");
}

void	print_bits(void *a, int n_bits)
{
	int		i;

	i = n_bits - 1;
	while (i >= 0)
	{
		printf("%i", bit_is_set(*((int *)a), i));
		i--;
	}
}

void	print_float_bits(float a)
{
	printf("% 10f = ", a);
	print_bits(&a, 32);
	printf("\n");
}

void	print_char_bits(char c)
{
	print_bits(&c, 8);
	printf("\n");
}

float	bin2float(char *bit_values)
{
	int i;
	int	nb;

	nb = 0;
	i = 0;
	while (i < 32)
	{
		nb += (1 << i) * (bit_values[31-i] == '1');
		i++;
	}
	return (*((float *)&(nb)));
}

int		two_to_the_nb(int nb)
{
	int		i;
	int		res;

	i = 0;
	res = 1;
	while (i < nb)
	{
		res += 1 << i;
		i++;
	}
	return (res);
}

long long		my_pow(int base, int power)
{
	long long		res;

	if (base == 0 && power == 0)
		return (1);
	if (base == 0)
		return (0);
	res = 1;
	while (power-- > 0)
		res *= base;
	return (res);
}

#define EXPONENT_MASK 0x7f800000
#define MANTISSA_MASK 0x007fffff

int		get_exponent(float a)
{
	int		e;

	e = (*(int *)&a & EXPONENT_MASK) >> 23;
	return (e);
}

int		get_mantissa(float a)
{
	int		m;

	m = *(int *)&a & MANTISSA_MASK;
	return (m);
}

#define FLOAT_EXPONENT_BIAS 127

int		main(void)
{
	float	a;
	int		exponent;
	int		mantissa;
	int		unbiased;
	int		neg_pow;
	long long	big;
	int		is_subnormal;

	a = 0.000000000000000000000000000000000000000000005;
	printf("%f\n", a);
	exponent = get_exponent(a);
	mantissa = get_mantissa(a);
	print_float_bits(a);
	printf("              ");
	print_bits(&exponent, 8);
	printf(" ");
	print_bits(&mantissa, 23);
	printf("\n");
	is_subnormal = (exponent == 0);
	unbiased = is_subnormal + exponent - FLOAT_EXPONENT_BIAS;
	big = mantissa + (1 << 23) * (!is_subnormal);
	printf("mantissa = %d, exponent = %d, unbiased = %d\n", mantissa, exponent, unbiased);
	print_bits(&big, 32);
	printf("\n");
	neg_pow = -23;
	while ((big & 1) == 0)
	{
		big >>= 1;
		neg_pow++;
	}
	if (unbiased < 0)
	{
		neg_pow += unbiased;
		unbiased = 0;
	}
	big = big * my_pow(5, -neg_pow) * my_pow(2, unbiased);
	printf("big = %lld neg_pow = %d\n", big, neg_pow);
	//printf("% .200f\n", bin2float("0""00000001""00000000000000000000001"));
	//printf("% .200f\n", bin2float("0""01111111""00000000000000000000001"));
	return (0);
}
