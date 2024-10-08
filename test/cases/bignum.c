#include <stdio.h>
#include "../includes/bignum.h"
#include "../print_bits.c"

static void	big_print_bits(t_big a)
{
	int		chunk_idx;

	chunk_idx = BIG_N_DIGITS - 1;
	while (chunk_idx >= 0)
	{
		print_bits(&(a.val[chunk_idx]), BIG_CHARS_PER_DIGIT * 8);
		printf(" ");
		chunk_idx--;
	}
	printf("\n");
}

/*
//
// big_sub is not implemented yet...
//
void test_big_sub(void)
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
*/

int main(void)
{
	printf("\n=== Bignums ===\n");
	return (0);
}
