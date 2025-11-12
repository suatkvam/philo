#include "philo.h"

void	init_mutexes_and_philos(t_philo_data *philo_data,
		long long must_eat_value)
{
	int	i;
	int	n;

	pthread_mutex_init(&philo_data->data_lock, NULL);
	pthread_mutex_init(&philo_data->write_lock, NULL);
	pthread_mutex_init(&philo_data->start_gate_lock, NULL);
	pthread_mutex_lock(&philo_data->start_gate_lock);
	philo_data->ready_philos_count = 0;
	i = -1;
	n = philo_data->number_of_philosophers;
	while (++i < n)
	{
		philo_data->philos[i].id = i + 1;
		philo_data->philos[i].eat_count = 0;
		philo_data->philos[i].must_eat_target = must_eat_value;
		philo_data->philos[i].data = philo_data;
		pthread_mutex_init(&philo_data->forks[i], NULL);
		if (n == 1)
		{
			philo_data->philos[i].left_fork = &philo_data->forks[i];
			philo_data->philos[i].right_fork = NULL;
		}
		else if (philo_data->philos[i].id % 2 == 0)
		{
			philo_data->philos[i].left_fork = &philo_data->forks[i];
			philo_data->philos[i].right_fork = &philo_data->forks[(i + 1) % n];
		}
		else
		{
			philo_data->philos[i].left_fork = &philo_data->forks[(i + 1) % n];
			philo_data->philos[i].right_fork = &philo_data->forks[i];
		}
	}
}
