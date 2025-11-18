/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:49:42 by akivam            #+#    #+#             */
/*   Updated: 2025/11/18 14:44:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

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
	philo_life_cycle(philo);
	return (NULL);
}
