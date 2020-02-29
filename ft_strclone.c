#include <stdlib.h>
#include "libftprintf.h"

char	*ft_strclone(const char *src)
{
	char	*str;
	int		len;
	int		i;

	len = ft_strlen((char *)src);
	/*
	if (len == INT_MAX)
		return (NULL);
		*/
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = src[i];
		i++;
	}
	str[len] = '\0';
	return (str);
}
