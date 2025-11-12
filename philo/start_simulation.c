/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:21:43 by akivam            #+#    #+#             */
/*   Updated: 2025/11/12 21:23:13 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

static void	wait_all_philos_ready(t_philo_data *philo_data)
{
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
}

void	start_simulation(t_philo_data *philo_data)
{
	int	i;

	i = -1;
	while (++i < philo_data->number_of_philosophers)
		pthread_create(&philo_data->philos[i].thread_id, NULL, philo_routine,
			&philo_data->philos[i]);
	wait_all_philos_ready(philo_data);
	philo_data->start_time = get_current_time();
	i = -1;
	while (++i < philo_data->number_of_philosophers)
		philo_data->philos[i].last_eat_time = philo_data->start_time;
	pthread_mutex_unlock(&philo_data->start_gate_lock);
}
