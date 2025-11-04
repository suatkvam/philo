#include "philo.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int	parse_data(int argc, char const *argv[], t_philo_data *philo_data, long long *must_eat_value)
{
	philo_data->number_of_philosophers = ft_atoll(argv[1]);
	philo_data->time_to_die = ft_atoll(argv[2]);
	philo_data->time_to_eat = ft_atoll(argv[3]);
	philo_data->time_to_sleep = ft_atoll(argv[4]);
	philo_data->is_dead = 0;
	*must_eat_value = -1;
	if (argc == 6)
		*must_eat_value = ft_atoll(argv[5]);
	if (philo_data->number_of_philosophers <= 0 || philo_data->time_to_die <= 0
		|| philo_data->time_to_eat <= 0 || philo_data->time_to_sleep <= 0
		|| (argc == 6 && *must_eat_value <= 0))
	{
		printf("Error: Invalid argument value (must be > 0 and no overflow)\n");
		return (1);
	}
	return (0);
}

int	allocate_resources(t_philo_data *philo_data)
{
	philo_data->philos = malloc(sizeof(t_philo)
			* philo_data->number_of_philosophers);
	philo_data->forks = malloc(sizeof(pthread_mutex_t)
			* philo_data->number_of_philosophers);
	if (!philo_data->philos || !philo_data->forks)
	{
		printf("Error: Malloc faild \n");
		free(philo_data->philos);
		free(philo_data->forks);
		return (1);
	}
	return (0);
}

void	init_mutexes_and_philos(t_philo_data *philo_data, long long must_eat_value)
{
	int	i;

	pthread_mutex_init(&philo_data->data_lock, NULL);
	pthread_mutex_init(&philo_data->write_lock, NULL);
	i = -1;
	while (++i < philo_data->number_of_philosophers)
	{
		philo_data->philos[i].id = i + 1;
		philo_data->philos[i].eat_count = 0;
		philo_data->philos[i].must_eat_target = must_eat_value;
		philo_data->philos[i].data = philo_data;
		pthread_mutex_init(&philo_data->forks[i], NULL);
	}
}

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
	return (0);
}
