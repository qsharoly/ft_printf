#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <limits.h>

int		bit_is_set(unsigned long nb, int pos)
{
	return ((nb & (1L << pos)) != 0);
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

double	bin2double(char *bit_values)
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

#define F64_EXPONENT_MASK 0x7ff0000000000000
#define F64_MANTISSA_MASK 0x000fffffffffffff
#define F64_EXPONENT_SIZE 11
#define F64_MANTISSA_SIZE 52
#define F64_EXPONENT_BIAS 1023

long	get_exponent(double a)
{
	long	e;

	e = (*(long *)&a & F64_EXPONENT_MASK) >> F64_MANTISSA_SIZE;
	return (e);
}

long	get_mantissa(double a)
{
	long	m;

	m = *(long *)&a & F64_MANTISSA_MASK;
	return (m);
}


#define	BIG_N_CHUNKS 4
#define CHUNK_TOP_BIT 63
#define CHUNK_N_BITS 64
#define HIGHEST_BIT_MASK (1L << CHUNK_TOP_BIT)
#define CHUNK_MAX_VALUE ULONG_MAX

typedef unsigned long	t_chunk;

typedef	struct		s_big
{
	t_chunk	val[BIG_N_CHUNKS];
}					t_big;

t_big	carry(t_big a, unsigned i)
{
	while (a.val[i] == CHUNK_MAX_VALUE && i < BIG_N_CHUNKS - 1)
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
		if (a.val[i] > CHUNK_MAX_VALUE - b.val[i])
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
			if (b.val[i] & (1L << j))
				res = big_add(res, a);
			a = big_shl_one(a);
			j++;
		}
		i++;
	}
	return (res);
}

int		big_cmp(t_big a, t_big b)
{
	unsigned	i;

	i = BIG_N_CHUNKS - 1;
	while (i >= 0)
	{
		if (a.val[i] < b.val[i])
			return (-1);
		if (a.val[i] > b.val[i])
			return (1);
		if (i == 0)
			break ;
		i--;
	}
	return (0);
}

t_big	big_from_chunk(t_chunk small)
{
	t_big	big;

	bzero(&big, sizeof(t_big));
	big.val[0] = small;
	return (big);
}

typedef struct		s_big_divmod_small
{
	t_big	factor;
	t_chunk	remainder;
}					t_big_divmod_small;

t_big_divmod_small	big_divmod_small(t_big top, t_chunk bot)
{
	t_big_divmod_small	out;
	t_big	accum;

	bzero(&out, sizeof(t_big_divmod_small));
	accum = big_from_chunk(bot);
	while (big_cmp(top, accum) > 0)
	{
		out.factor = big_add(out.factor, big_from_chunk(1));
		accum = big_add(accum, big_from_chunk(bot));
	}
	out.remainder = top.val[0] % bot;
	return (out);
}

#define BIG_BUFSIZE 100
void	big_print(t_big a)
{
	t_big_divmod_small	intermediary;
	char				buf[BIG_BUFSIZE];
	int					i;

	intermediary = big_divmod_small(a, 10);
	buf[BIG_BUFSIZE - 1] = '\0';
	i = 2;
	while (big_cmp(intermediary.factor, big_from_chunk(0)) > 0)
	{
		buf[BIG_BUFSIZE - i] = '0' + (char)intermediary.remainder;
		intermediary = big_divmod_small(intermediary.factor, 10);
		i++;
	}
	printf("%s\n", &buf[BIG_BUFSIZE - i + 1]);
}

void	big_print_bits(t_big a)
{
	int		i;

	i = BIG_N_CHUNKS - 1;
	while (i >= 0)
	{
		print_bits(&(a.val[i]), CHUNK_N_BITS);
		i--;
	}
}

t_big	generate_pow(t_chunk small_base, t_chunk power)
{
	t_big	res;
	t_big	base;

	if (small_base == 0 && power == 0)
		return (big_from_chunk(1));
	if (small_base == 0)
		return (big_from_chunk(0));
	res = big_from_chunk(1);
	base = big_from_chunk(small_base);
	while (power-- > 0)
		res = big_mul(res, base);
	return (res);
}


int		main(void)
{
	double	a;
	long	exponent;
	unsigned long	mantissa;
	long	unbiased;
	long	neg_pow;
	unsigned long	small;
	t_big	big;
	int		is_subnormal;

	a = 0.125;
	printf("%f\n", a);
	exponent = get_exponent(a);
	mantissa = get_mantissa(a);
	print_double_bits(a);
	printf("              ");
	print_bits(&exponent, F64_EXPONENT_SIZE);
	printf(" ");
	print_bits(&mantissa, F64_MANTISSA_SIZE);
	printf("\n");
	is_subnormal = (exponent == 0);
	unbiased = is_subnormal + exponent - F64_EXPONENT_BIAS;
	small = mantissa + (1L << F64_MANTISSA_SIZE) * (!is_subnormal);
	printf("mantissa = %lu, exponent = %ld, unbiased = %ld\n", mantissa, exponent, unbiased);
	print_bits(&small, 64);
	printf("\n");
	neg_pow = -F64_MANTISSA_SIZE;
	while ((small & 1L) == 0)
	{
		small >>= 1;
		neg_pow++;
	}
	if (unbiased < 0)
	{
		neg_pow += unbiased;
		unbiased = 0;
	}
	big = big_mul(big_from_chunk(small), generate_pow(5, (unsigned long)-neg_pow));
	big = big_mul(big, generate_pow(2, (unsigned long)unbiased));
	big_print_bits(big);
	printf("\n");
	printf("%lu x 10^%ld\n", big.val[0], neg_pow);
	big_print(big);
	return (0);
}
