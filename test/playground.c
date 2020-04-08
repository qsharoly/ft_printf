#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <limits.h>

int		bit_is_set(unsigned nb, unsigned pos)
{
	return ((nb & (1 << pos)) != 0);
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
		printf("%i", bit_is_set(*((unsigned *)a), i));
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

#define	BIG_N_CHUNKS 4
#define CHUNK_TOP_BIT 31
#define CHUNK_N_BITS 32
#define HIGHEST_BIT_MASK (1 << CHUNK_TOP_BIT)

typedef	struct		s_big
{
	unsigned	val[BIG_N_CHUNKS];
}					t_big;

t_big	carry(t_big a, unsigned i)
{
	while (a.val[i] == UINT_MAX && i < BIG_N_CHUNKS - 1)
		i++;
	a.val[i] += 1;
	return (a);
}

t_big	big_add(t_big a, t_big b)
{
	t_big		sum;
	unsigned	i;

	bzero(&sum, sizeof(sum));
	i = 0;
	while (i < BIG_N_CHUNKS)
	{
		sum.val[i] = a.val[i] + b.val[i];
		if (a.val[i] > UINT_MAX - b.val[i])
			a = carry(a, i + 1);
		i++;
	}
	return (sum);
}

t_big	big_shl_one(t_big a)
{
	unsigned	i;
	int			carry;
	int			hi_is_set;

	i = 0;
	carry = 0;
	while (i < BIG_N_CHUNKS)
	{
		hi_is_set = a.val[i] & HIGHEST_BIT_MASK;
		a.val[i] <<= 1;
		if (carry)
			a.val[i] += 1;
		carry = hi_is_set;
		i++;
	}
	return (a);
}

void	big_copy(t_big *dst, const t_big *src)
{
	unsigned	i;

	i = 0;
	while (i < BIG_N_CHUNKS)
	{
		dst->val[i] = src->val[i];
		i++;
	}
}

t_big	big_mul(t_big a, t_big b)
{
	t_big		res;
	unsigned	i;
	unsigned	j;

	bzero(&res, sizeof(res));
	i = 0;
	while (i < BIG_N_CHUNKS)
	{
		j = 0;
		while (j < CHUNK_N_BITS)
		{
			if (bit_is_set(b.val[i], j))
				res = big_add(res, a);
			a = big_shl_one(a);
			j++;
		}
		i++;
	}
	return (res);
}

void	big_print_bits(t_big a)
{
	int		i;

	i = BIG_N_CHUNKS - 1;
	while (i >= 0)
	{
		print_bits(&(a.val[i]), 32);
		i--;
	}
}

t_big	big_from_small(unsigned small)
{
	t_big	big;

	bzero(&big, sizeof(t_big));
	big.val[0] = small;
	return (big);
}

t_big	generate_pow(unsigned small_base, unsigned power)
{
	t_big	res;
	t_big	base;

	if (small_base == 0 && power == 0)
		return (big_from_small(1));
	if (small_base == 0)
		return (big_from_small(0));
	res = big_from_small(1);
	base = big_from_small(small_base);
	while (power-- > 0)
		res = big_mul(res, base);
	return (res);
}

#define FLOAT_EXPONENT_BIAS 127

int		main(void)
{
	float	a;
	int		exponent;
	int		mantissa;
	int		unbiased;
	int		neg_pow;
	unsigned	small;
	t_big	big;
	int		is_subnormal;

	a = 0.000000000000000000000000000000000000000000005;
	a = bin2float("0""00000001""00000000000000000000001");
	a = 5e+10;
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
	small = mantissa + (1 << 23) * (!is_subnormal);
	printf("mantissa = %d, exponent = %d, unbiased = %d\n", mantissa, exponent, unbiased);
	print_bits(&small, 32);
	printf("\n");
	neg_pow = -23;
	while ((small & 1) == 0)
	{
		small >>= 1;
		neg_pow++;
	}
	if (unbiased < 0)
	{
		neg_pow += unbiased;
		unbiased = 0;
	}
	big = big_mul(big_from_small(small), generate_pow(5, (unsigned)-neg_pow));
	big = big_mul(big, generate_pow(2, (unsigned)unbiased));
	big_print_bits(big);
	printf("\n");
	printf("%u x 10^-%u\n", big.val[0], (unsigned)-neg_pow);
	return (0);
}
