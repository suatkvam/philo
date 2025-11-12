#include "philo.h"
#include "utils/utils.h"

void	write_status(t_philo *philo, const char *status)
{
	long long	timestamp;
	int			is_dead;

	pthread_mutex_lock(&philo->data->data_lock);
	is_dead = philo->data->is_dead;
	pthread_mutex_unlock(&philo->data->data_lock);
	pthread_mutex_lock(&philo->data->write_lock);
	timestamp = get_current_time() - philo->data->start_time;
	if (is_dead == 0)
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	else if (status[0] == 'd')
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
}
