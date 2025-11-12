#include "philo.h"
#include "utils/utils.h"

void	start_simulation(t_philo_data *philo_data)
{
	int	i;

	i = -1;
	while (++i < philo_data->number_of_philosophers)
		pthread_create(&philo_data->philos[i].thread_id, NULL, philo_routine,
			&philo_data->philos[i]);
	while (1)
	{
		pthread_mutex_lock(&philo_data->data_lock);
		if (philo_data->ready_philos_count == philo_data->number_of_philosophers)
		{
			pthread_mutex_unlock(&philo_data->data_lock);
			break ;
		}
		pthread_mutex_unlock(&philo_data->data_lock);
		usleep(100);
	}
	philo_data->start_time = get_current_time();
	i = -1;
	while (++i < philo_data->number_of_philosophers)
		philo_data->philos[i].last_eat_time = philo_data->start_time;
	pthread_mutex_unlock(&philo_data->start_gate_lock);
}
