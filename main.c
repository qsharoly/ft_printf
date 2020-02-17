#include "libftprintf.h"
#include <stdio.h>

int		main(void)
{
	int	i;
	/*
	//simple cases
	ft_printf("just a string\n");
	ft_printf("percent sign%%ef\n");
	ft_printf("just an int %d def\n", 5);
	ft_printf("%s well\n", "very");
	ft_printf("my %s equals %d%%\n", "value", -123);
	*/
	ft_printf("abc %x %X %x %X", 42, 42, -42, -42);
	//prefixes
	ft_printf("High hex %#X\n", 255);
	ft_printf("Lower hex %#x\n", 243);
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
	printf("%-010dignore\n", 19);
	/*
	//negative cases
	ft_printf("no type % 0ratata %d\n", 13, 14);
	printf("no type % 0ratata %d\n", 15, 16);
	ft_printf("type mi%smatch\n", 100500);//segfault
	ft_printf("what if %d\n", "i put a string there");//print some integer
	*/
	return (0);
}
