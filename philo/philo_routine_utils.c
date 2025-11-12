/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:29:01 by akivam            #+#    #+#             */
/*   Updated: 2025/11/12 21:29:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

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
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	sleep_routine(t_philo *philo)
{
	write_status(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

void	think_routine(t_philo *philo)
{
	write_status(philo, "is thinking");
	usleep(1000);
}

int	is_simulation_over(t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(&philo->data->data_lock);
	is_dead = philo->data->is_dead;
	pthread_mutex_unlock(&philo->data->data_lock);
	return (is_dead);
}

void	philo_life_cycle(t_philo *philo)
{
	while (1)
	{
		if (is_simulation_over(philo))
			break ;
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
	}
}
