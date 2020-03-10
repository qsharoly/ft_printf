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

unsigned char	get_exponent(float a)
{
	unsigned char	exp;
	int		i;

	exp = 0;
	i = 0;
	while (i < 8)
	{
		exp += bit_is_set(*((int *)&a), i + 23) << i;
		i++;
	}
	return (exp - 127);
}

float	make_from_binary(char *bit_values)
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
	return (*((float *)&nb));
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

int		main(void)
{
	/*
	float	a;

	a = -10. + .15625;
	while (++a < 10)
	{
		print_float_bits(a);
		printf("              ");
		print_char_bits(get_exponent(a));
		printf("% 10i\n", two_to_the_nb(get_exponent(a)));
	}
	*/
	printf("% .200f\n", make_from_binary("0""00000001""00000000000000000000001"));
	printf("% .200f\n", make_from_binary("0""01111111""00000000000000000000001"));
	printf("%+d\n%+d\n%+d\n", -1, 0, 1);
	printf("% d\n% d\n% d\n", -1, 0, 1);
	printf("%#0o\n", 127);
	printf("%#0o\n", 0);
	write(1, NULL, 0);
	write(1, NULL, 10);
	return (0);
}
