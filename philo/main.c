#include "philo.h"

int	main(int argc, char const *argv[])
{
	t_philo_data	philo_data;
	long long		must_eat_value;

	if (validate_arguments(argc, argv) != 0)
		return (1);
	if (parse_data(argc, argv, &philo_data, &must_eat_value) != 0)
		return (1);
	if (allocate_resources(&philo_data) != 0)
		return (1);
	init_mutexes_and_philos(&philo_data, must_eat_value);
	start_simulation(&philo_data);
	observer(&philo_data);
	clean_resources(&philo_data);
	return (0);
}
