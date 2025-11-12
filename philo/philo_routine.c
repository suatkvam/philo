/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:21:23 by akivam            #+#    #+#             */
/*   Updated: 2025/11/12 22:12:50 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

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
