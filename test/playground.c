#include <stdio.h>
#include <stdlib.h>
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


#define	BIG_N_CHUNKS 8
#define CHUNK_TOP_BIT 63
#define CHUNK_N_BITS 64
#define HIGHEST_BIT_MASK (1L << CHUNK_TOP_BIT)
#define CHUNK_MAX_VALUE ULONG_MAX

typedef unsigned long	t_chunk;

typedef	struct		s_big
{
	t_chunk	val[BIG_N_CHUNKS];
}					t_big;

t_big	big_zero(void)
{
	t_big		a;

	bzero(&a, sizeof(a));
	return (a);
}

t_big	big_carry(t_big a, unsigned chunk_idx)
{
	while (a.val[chunk_idx] == CHUNK_MAX_VALUE && chunk_idx < BIG_N_CHUNKS - 1)
		chunk_idx++;
	a.val[chunk_idx] += 1;
	return (a);
}

t_big	big_add(t_big a, t_big b)
{
	t_big		sum;
	unsigned	chunk_idx;

	sum = big_zero();
	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		sum.val[chunk_idx] = a.val[chunk_idx] + b.val[chunk_idx];
		if (a.val[chunk_idx] > CHUNK_MAX_VALUE - b.val[chunk_idx])
			a = big_carry(a, chunk_idx + 1);
		chunk_idx++;
	}
	return (sum);
}

t_big	big_inc(t_big a)
{
	a = big_carry(a, 0);
	return (a);
}

t_big	big_shl_one(t_big a)
{
	unsigned	chunk_idx;
	int			need_carry;
	int			hi_is_set;

	chunk_idx = 0;
	need_carry = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		hi_is_set = a.val[chunk_idx] & HIGHEST_BIT_MASK;
		a.val[chunk_idx] <<= 1;
		if (need_carry)
			a.val[chunk_idx] += 1;
		need_carry = hi_is_set;
		chunk_idx++;
	}
	return (a);
}

t_big	big_shr_one(t_big a)
{
	unsigned	chunk_idx;
	int			need_carry;

	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		need_carry = (chunk_idx < BIG_N_CHUNKS - 1) && (a.val[chunk_idx + 1] & 1L);
		a.val[chunk_idx] >>= 1;
		if (need_carry)
			a.val[chunk_idx] |= HIGHEST_BIT_MASK;
		chunk_idx++;
	}
	return (a);
}

t_big	big_mul(t_big a, t_big b)
{
	t_big		res;
	unsigned	chunk_idx;
	unsigned	j;

	res = big_zero();
	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		j = 0;
		while (j < CHUNK_N_BITS)
		{
			if (b.val[chunk_idx] & (1L << j))
				res = big_add(res, a);
			a = big_shl_one(a);
			j++;
		}
		chunk_idx++;
	}
	return (res);
}

int		big_cmp(t_big a, t_big b)
{
	unsigned	chunk_idx;

	chunk_idx = BIG_N_CHUNKS - 1;
	while (chunk_idx >= 0)
	{
		if (a.val[chunk_idx] < b.val[chunk_idx])
			return (-1);
		if (a.val[chunk_idx] > b.val[chunk_idx])
			return (1);
		if (chunk_idx == 0)
			break ;
		chunk_idx--;
	}
	return (0);
}

t_big	big_from_chunk(t_chunk small)
{
	t_big	big;

	big = big_zero();
	big.val[0] = small;
	return (big);
}

void			big_print_bits(t_big);

t_big	big_sub(t_big a, t_big b)
{
	t_big		diff;
	unsigned	chunk_idx;

	diff = big_zero();
	chunk_idx = 0;
	while (chunk_idx < BIG_N_CHUNKS)
	{
		diff.val[chunk_idx] = a.val[chunk_idx] - b.val[chunk_idx];
		if (a.val[chunk_idx] < b.val[chunk_idx])
			b = big_carry(b, chunk_idx + 1);
		chunk_idx++;
	}
	return (diff);
}

unsigned		big_top_bit(t_big a)
{
	unsigned	chunk_idx;
	t_chunk		top_chunk;
	unsigned	i;

	chunk_idx = BIG_N_CHUNKS - 1;
	while (a.val[chunk_idx] == 0)
		chunk_idx--;
	i = chunk_idx * CHUNK_N_BITS;
	top_chunk = a.val[chunk_idx];
	while (top_chunk)
	{
		top_chunk >>= 1;
		i++;
	}
	return (i);
}

typedef struct		s_big_divmod
{
	t_big	quo;
	t_big	rem;
}					t_big_divmod;

t_big_divmod	big_divmod(t_big top, t_big bot)
{
	t_big_divmod	out;
	unsigned		shift;
	unsigned		i;

	if (big_cmp(top, bot) < 0)
	{
		out.quo = big_zero();
		out.rem = top;
		return (out);
	}
	shift = big_top_bit(top) - big_top_bit(bot);
	i = 0;
	while (i < shift)
	{
		bot = big_shl_one(bot);
		i++;
	}
	out.quo = big_zero();
	i = shift;
	while (i >= 0)
	{
		if (big_cmp(top, bot) >= 0)
		{
			top = big_sub(top, bot);
			out.quo.val[i / CHUNK_N_BITS] |= (1L << (i % CHUNK_N_BITS));
		}
		bot = big_shr_one(bot);
		if (i == 0)
			break ;
		i--;
	}
	out.rem = top;
	return (out);
}

#define BIG_BUFSIZE 50
char	*big_to_string(t_big a)
{
	t_big_divmod	tmp;
	t_big			radix;
	t_big			zero;
	char			buf[BIG_BUFSIZE];
	char			*s;
	int				i;

	tmp.quo = a;
	zero = big_zero();
	radix = big_from_chunk(10);
	i = 0;
	while (big_cmp(tmp.quo, zero) > 0)
	{
		tmp = big_divmod(tmp.quo, radix);
		buf[BIG_BUFSIZE - 1 - i] = '0' + (char)tmp.rem.val[0];
		i++;
	}
	s = malloc(i + 1);
	s[i] = '\0';
	memcpy(s, &buf[BIG_BUFSIZE - i], i);
	return (s);
}

void	big_print_bits(t_big a)
{
	int		chunk_idx;

	chunk_idx = BIG_N_CHUNKS - 1;
	while (chunk_idx >= 0)
	{
		print_bits(&(a.val[chunk_idx]), CHUNK_N_BITS);
		printf(" ");
		chunk_idx--;
	}
	printf("\n");
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

int		i_max(int a, int b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

char	*finalize(char *digits, int decimal_place, int precision)
{
	char			*ipart;
	char			*fpart;
	char			*s;
	int				i;
	int				fsize;

	i = strlen(digits);
	if (decimal_place < 0)
	{
		decimal_place = -decimal_place;
		fsize =	i_max(decimal_place, precision);
		fpart = malloc(fsize + 1);
		memset(fpart, '0', fsize);
		fpart[fsize] = '\0';
		memcpy(fpart, digits + i - decimal_place, decimal_place);
		if (decimal_place >= i)
		{
			ipart = strdup("0");
			memset(fpart, '0', decimal_place - i);
		}
		else
		{
			ipart = malloc(i - decimal_place);
			memcpy(ipart, digits, (i - decimal_place));
		}
	}
	else
	{
		fpart = malloc(precision + 1);
		fpart[precision] = '\0';
		memset(fpart, '0', precision);
		ipart = malloc(i + decimal_place + 1);
		ipart[i + decimal_place] = '\0';
		memcpy(ipart, digits, i);
		memset(ipart + i, '0', decimal_place);
	}
	fpart[precision] = '\0';
	s = malloc(strlen(ipart) + strlen(".") + strlen(fpart) + 1);
	strcpy(s, ipart);
	strcat(s, ".");
	strcat(s, fpart);
	free(ipart);
	free(fpart);
	return (s);
}

char	*pf_dtoa(double d, int precision)
{
	char			*s;
	long			exponent;
	unsigned long	mantissa;
	long			dec_pow;
	t_big			big;
	int				is_subnormal;

	exponent = get_exponent(d);
	mantissa = get_mantissa(d);
	is_subnormal = (exponent == 0);
	exponent = is_subnormal + exponent - F64_EXPONENT_BIAS;
	mantissa = mantissa + (!is_subnormal) * (1L << F64_MANTISSA_SIZE);
	dec_pow = -F64_MANTISSA_SIZE;
	while ((mantissa & 1L) == 0)
	{
		mantissa >>= 1;
		dec_pow++;
	}
	if (exponent < 0)
	{
		dec_pow += exponent;
		exponent = 0;
	}
#if 1
	printf("shifted mantissa = %lu, exponent = %ld, power = %ld\n", mantissa, exponent, dec_pow);
#endif
	big = big_mul(big_from_chunk(mantissa), generate_pow(5, (unsigned long)-dec_pow));
	big = big_mul(big, generate_pow(2, (unsigned long)exponent));
	printf("%s * 10^%ld\n", big_to_string(big), dec_pow);
	s = finalize(big_to_string(big), dec_pow, precision);
	return (s);
}

void	test_big_sub(void)
{
	t_big	a;
	t_big	b;
	t_big	diff;
	int		check;

	a = big_zero();
	a.val[0] = 100;
	a.val[1] = 24;
	b = big_zero();
	b.val[0] = 120;
	b.val[1] = 127;
	diff = big_sub(a, b);
	check = big_cmp(a, big_add(diff, b));
	big_print_bits(a);
	printf("\n");
	big_print_bits(b);
	printf("\n");
	big_print_bits(diff);
	printf("\n");
	printf("cmp(a, diff + b) returned %d\n", check);
}

int		main(void)
{
	double	a;

	a = 0.3;
	print_double_bits(a);
	printf("%f\n", a);
	printf("%s\n", pf_dtoa(a, 6));
	return (0);
}
