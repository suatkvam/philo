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

void	observer(t_philo_data *philo_data)
{
	int	i;
	int	full_philo_cnt;

	while (philo_data->is_dead == 0)
	{
		full_philo_cnt = 0;
		i = -1;
		while (++i < philo_data->number_of_philosophers)
		{
			pthread_mutex_lock(&philo_data->data_lock);
			if (get_current_time()
				- philo_data->philos[i].last_eat_time > philo_data->time_to_die)
			{
				philo_data->is_dead = 1;
				pthread_mutex_unlock(&philo_data->data_lock);
				write_status(&philo_data->philos[i], "died");
				return ;
			}
			if (philo_data->philos[i].must_eat_target != -1
				&& philo_data->philos[i].eat_count >= philo_data->philos[i].must_eat_target)
				full_philo_cnt++;
			pthread_mutex_unlock(&philo_data->data_lock);
		}
		if (philo_data->philos[0].must_eat_target != -1
			&& full_philo_cnt == philo_data->number_of_philosophers)
		{
			pthread_mutex_lock(&philo_data->data_lock);
			philo_data->is_dead = 1;
			pthread_mutex_unlock(&philo_data->data_lock);
			return ;
		}
		usleep(1000);
	}
	return ;
}
