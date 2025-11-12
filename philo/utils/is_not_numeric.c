#include "error.h"
#include <stdio.h>

int	is_not_numeric(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' ||(str[i]>= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		return (1);
	if (str[i] == '+')
		i++;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (1);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] != '\0')
		return (1);
	return (0);
}
