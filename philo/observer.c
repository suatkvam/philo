/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:49:42 by akivam            #+#    #+#             */
/*   Updated: 2025/11/18 14:44:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

static int	check_philo_death(t_philo_data *philo_data, int i)
{
	pthread_mutex_lock(&philo_data->data_lock);
	if (get_current_time()
		- philo_data->philos[i].last_eat_time > philo_data->time_to_die)
	{
		philo_data->is_dead = 1;
		pthread_mutex_unlock(&philo_data->data_lock);
		write_status(&philo_data->philos[i], "died");
		return (1);
	}
	pthread_mutex_unlock(&philo_data->data_lock);
	return (0);
}

static int	check_philo_full(t_philo_data *philo_data, int i)
{
	int	is_full;

	pthread_mutex_lock(&philo_data->data_lock);
	is_full = 0;
	if (philo_data->philos[i].must_eat_target != -1
		&& philo_data->philos[i].eat_count
		>= philo_data->philos[i].must_eat_target)
		is_full = 1;
	pthread_mutex_unlock(&philo_data->data_lock);
	return (is_full);
}

static int	check_all_philos_full(t_philo_data *philo_data, int full_philo_cnt)
{
	if (philo_data->philos[0].must_eat_target != -1
		&& full_philo_cnt == philo_data->number_of_philosophers)
	{
		pthread_mutex_lock(&philo_data->data_lock);
		philo_data->is_dead = 1;
		pthread_mutex_unlock(&philo_data->data_lock);
		return (1);
	}
	return (0);
}

void	observer(t_philo_data *philo_data)
{
	int	i;
	int	full_philo_cnt;
	int	is_dead;

	is_dead = 0;
	while (is_dead == 0)
	{
		full_philo_cnt = 0;
		i = -1;
		while (++i < philo_data->number_of_philosophers)
		{
			if (check_philo_death(philo_data, i))
				return ;
			if (check_philo_full(philo_data, i))
				full_philo_cnt++;
		}
		if (check_all_philos_full(philo_data, full_philo_cnt))
			return ;
		pthread_mutex_lock(&philo_data->data_lock);
		is_dead = philo_data->is_dead;
		pthread_mutex_unlock(&philo_data->data_lock);
		usleep(1000);
	}
	return ;
}
