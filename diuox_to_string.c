#include "libftprintf.h"

void signed_to_string(char **str, char **prefix, t_fmt fmt, long long int nb)
{
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strclone("");
	else
		*str = ft_itoa_base_abs(nb, 10, fmt.precision, 0);
	if (nb >= 0)
	{
		if (flag_is_set(fmt.flags, PLUS_POSITIVE))
			*prefix = ft_strclone("+");
		else if (flag_is_set(fmt.flags, SPACE_POSITIVE))
			*prefix = ft_strclone(" ");
		else
			*prefix = ft_strclone("");
	}
	else
		*prefix = ft_strclone("-");
}

void	unsigned_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb)
{
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strclone("");
	else
		*str = ft_itoa_base_unsigned(nb, 10, fmt.precision, 0);
	*prefix = ft_strclone("");
}

void	octal_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb)
{
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strclone("");
	else
		*str = ft_itoa_base_unsigned(nb, 8, fmt.precision, 0);
	if (flag_is_set(fmt.flags, ALTERNATE_FORM) && ((nb != 0 && fmt.precision <= 0) || (nb == 0 && fmt.precision == 0)))
		*prefix = ft_strclone("0");
	else
		*prefix = ft_strclone("");
}

void hex_to_string(char **str, char **prefix, t_fmt fmt, unsigned long long nb)
{
	int	upcase;

	upcase = (fmt.type == 'X');
	if (fmt.precision == 0 && nb == 0)
		*str = ft_strclone("");
	else
		*str = ft_itoa_base_unsigned(nb, 16, fmt.precision, upcase);
	if (flag_is_set(fmt.flags, ALTERNATE_FORM) && nb != 0)
		*prefix = upcase ? ft_strclone("0X") : ft_strclone("0x");
	else
		*prefix = ft_strclone("");
}
