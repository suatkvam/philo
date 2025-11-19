/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:49:42 by akivam            #+#    #+#             */
/*   Updated: 2025/11/18 14:44:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

static void	precise_usleep(long long ms, t_philo_data *data)
{
	long long	start;
	long long	current;
	int			is_dead;

	start = get_current_time();
	while (1)
	{
		pthread_mutex_lock(&data->data_lock);
		is_dead = data->is_dead;
		pthread_mutex_unlock(&data->data_lock);
		if (is_dead)
			break ;
		current = get_current_time();
		if (current - start >= ms)
			break ;
		usleep(100);
	}
}

void	eat_routine(t_philo *philo)
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
	precise_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	sleep_routine(t_philo *philo)
{
	write_status(philo, "is sleeping");
	precise_usleep(philo->data->time_to_sleep, philo->data);
}

void	think_routine(t_philo *philo)
{
	long long	think_time;
	long long	num_philos;

	write_status(philo, "is thinking");
	num_philos = philo->data->number_of_philosophers;
	if (num_philos == 1)
		return ;
	if (num_philos % 2 == 0)
		return ;
	think_time = (philo->data->time_to_eat * 2) - philo->data->time_to_sleep;
	if (think_time < 0)
		think_time = 0;
	if (think_time > 0)
		precise_usleep(think_time, philo->data);
}
