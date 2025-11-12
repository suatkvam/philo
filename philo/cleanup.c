#include "philo.h"

void	clean_resources(t_philo_data *data)
{
	int	i;
	int	n;

	i = -1;
	n = data->number_of_philosophers;
	while (++i < n)
	{
		pthread_join(data->philos[i].thread_id, NULL);
	}
	i = -1;
	while ((++i < n))
	{
		pthread_mutex_destroy(&data->forks[i]);
	}
	pthread_mutex_destroy(&data->data_lock);
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->start_gate_lock);
	free(data->philos);
	free(data->forks);
}
