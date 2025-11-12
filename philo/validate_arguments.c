#include "philo.h"
#include "utils/utils.h"

int	validate_arguments(int argc, char const *argv[])
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (is_not_numeric(argv[i]) == 1)
		{
			printf("Error: Invalid argument format \n");
			return (1);
		}
		i++;
	}
	return (0);
}
