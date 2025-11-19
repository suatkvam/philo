/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes_and_philos.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:49:42 by akivam            #+#    #+#             */
/*   Updated: 2025/11/18 14:44:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_single_philo(t_philo_data *philo_data, int i,
		long long must_eat_value)
{
	int	n;

	n = philo_data->number_of_philosophers;
	philo_data->philos[i].id = i + 1;
	philo_data->philos[i].eat_count = 0;
	philo_data->philos[i].must_eat_target = must_eat_value;
	philo_data->philos[i].data = philo_data;
	pthread_mutex_init(&philo_data->forks[i], NULL);
	if (n == 1)
	{
		philo_data->philos[i].left_fork = &philo_data->forks[i];
		philo_data->philos[i].right_fork = NULL;
	}
	else if (philo_data->philos[i].id % 2 == 0)
	{
		philo_data->philos[i].left_fork = &philo_data->forks[i];
		philo_data->philos[i].right_fork = &philo_data->forks[(i + 1) % n];
	}
	else
	{
		philo_data->philos[i].left_fork = &philo_data->forks[(i + 1) % n];
		philo_data->philos[i].right_fork = &philo_data->forks[i];
	}
}

int	allocate_resources(t_philo_data *philo_data)
{
	philo_data->philos = malloc(sizeof(t_philo)
			* philo_data->number_of_philosophers);
	if (!philo_data->philos)
		return (1);
	philo_data->forks = malloc(sizeof(pthread_mutex_t)
			* philo_data->number_of_philosophers);
	if (!philo_data->forks)
	{
		free(philo_data->philos);
		return (1);
	}
	return (0);
}

void	init_mutexes_and_philos(t_philo_data *philo_data,
		long long must_eat_value)
{
	int	i;

	pthread_mutex_init(&philo_data->data_lock, NULL);
	pthread_mutex_init(&philo_data->write_lock, NULL);
	pthread_mutex_init(&philo_data->start_gate_lock, NULL);
	pthread_mutex_lock(&philo_data->start_gate_lock);
	philo_data->ready_philos_count = 0;
	i = -1;
	while (++i < philo_data->number_of_philosophers)
		init_single_philo(philo_data, i, must_eat_value);
}
