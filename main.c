#include "libftprintf.h"
#include <stdio.h>
#include <limits.h>

int		main(void)
{
	int	i;
	//simple cases
	ft_printf("just a string\n");
	ft_printf("percent sign%%ef\n");
	ft_printf("just an int %d def\n", 5);
	ft_printf("%s well\n", "very");
	ft_printf("my %s equals %d%%\n", "value", -123);
	ft_printf("abc %x %X %x %X", 42, 42, -42, -42);
	//prefixes
	ft_printf("Lower hex %#x\n", 243);
	ft_printf("High hex %#X\n", 255);
	/*
	i = -1;
	while (i <= 1)
	{
		ft_printf("Prefix pos with plus sign%+d\n", i);
		i++;
	}
	i = -1;
	while (i <= 1)
	{
		ft_printf("Prefix pos with a space % d\n", i);
		i++;
	}
	*/
	//min width and padding flags
	ft_printf("%10d\n", 12);
	ft_printf("%010d\n", 10);
	ft_printf("% 10d\n", 33);
	ft_printf("%-10dtons\n", 100);
	ft_printf("%-010dignorewhat\n", 23);
	ft_printf("zero precision shouldnt print zero%.o %.0o\n", 0, 0);
	//conversions
	ft_printf("%d\n", INT_MIN);
	ft_printf("%hhu\n", -1);
	ft_printf("%hu\n", -1);
	ft_printf("%u\n", -1);
	ft_printf("%lu\n", -1);
	ft_printf("%llu\n", ULLONG_MAX);
	printf("%hhu\n", -1);
	printf("%hu\n", -1);
	printf("%u\n", -1);
	printf("%lu\n", -1);
	printf("%llu\n", -1);
	printf("%llu\n", ULLONG_MAX);
	//precision
	ft_printf("%06.3i\n", 0);
	ft_printf("%06.3i\n", 1);
	printf("%06.3i\n", 0);
	printf("%06.3i\n", 1);
	ft_printf("%06.d %06.0d\n", 1, 2);
	printf("%06.d %06.0d\n", 1, 2);
	/*
	//negative cases
	ft_printf("no type % 0ratata %d\n", 13, 14);
	printf("no type % 0ratata %d\n", 15, 16);
	ft_printf("type mi%smatch\n", 100500);//segfault
	ft_printf("what if %d\n", "i put a string there");//print some integer
	*/
	return (0);
}
