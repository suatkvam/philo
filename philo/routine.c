#include "philo.h"
#include "utils/utils.h"

static void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	write_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	write_status(philo, "has taken a fork");
	write_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->data_lock);
	philo->last_eat_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

static void	sleep_routine(t_philo *philo)
{
	write_status(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

static void	think_routine(t_philo *philo)
{
	write_status(philo, "is thinking");
	usleep(100);
}

void	write_status(t_philo *philo, const char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->write_lock);
	timestamp = get_current_time() - philo->data->start_time;
	if (philo->data->is_dead == 0)
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	else if (status[0] == 'd')
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
}

void	*philo_routine(void *argument)
{
	t_philo	*philo;

	philo = (t_philo *)argument;
	pthread_mutex_lock(&philo->data->data_lock);
	philo->data->ready_philos_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	pthread_mutex_lock(&philo->data->start_gate_lock);
	pthread_mutex_unlock(&philo->data->start_gate_lock);
	if (philo->data->number_of_philosophers == 1)
	{
		write_status(philo, "has taken a fork");
		usleep(philo->data->time_to_die * 1000 + 1000);
		return (NULL);
	}
	while (philo->data->is_dead == 0)
	{
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
	}
	return (NULL);
}
