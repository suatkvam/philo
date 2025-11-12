/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_resources.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:21:05 by akivam            #+#    #+#             */
/*   Updated: 2025/11/12 21:21:06 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	allocate_resources(t_philo_data *philo_data)
{
	philo_data->philos = malloc(sizeof(t_philo)
			* philo_data->number_of_philosophers);
	philo_data->forks = malloc(sizeof(pthread_mutex_t)
			* philo_data->number_of_philosophers);
	if (!philo_data->philos || !philo_data->forks)
	{
		printf("Error: Malloc faild \n");
		free(philo_data->philos);
		free(philo_data->forks);
		return (1);
	}
	return (0);
}
