/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:49:42 by akivam            #+#    #+#             */
/*   Updated: 2025/11/18 14:44:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

int	parse_data(int argc, char const *argv[], t_philo_data *philo_data,
		long long *must_eat_value)
{
	philo_data->number_of_philosophers = ft_atoll(argv[1]);
	philo_data->time_to_die = ft_atoll(argv[2]);
	philo_data->time_to_eat = ft_atoll(argv[3]);
	philo_data->time_to_sleep = ft_atoll(argv[4]);
	philo_data->is_dead = 0;
	*must_eat_value = -1;
	if (argc == 6)
		*must_eat_value = ft_atoll(argv[5]);
	if (philo_data->number_of_philosophers <= 0 || philo_data->time_to_die <= 0
		|| philo_data->time_to_eat <= 0 || philo_data->time_to_sleep <= 0
		|| (argc == 6 && *must_eat_value <= 0))
	{
		printf("Error: Invalid argument value (must be > 0 and no overflow)\n");
		return (1);
	}
	return (0);
}
