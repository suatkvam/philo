/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:29:01 by akivam            #+#    #+#             */
/*   Updated: 2025/11/12 22:37:39 by akivam           ###   ########.fr       */
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
	long long	think_time;

	write_status(philo, "is thinking");
	// Tek filozof durumu
	if (philo->data->number_of_philosophers == 1)
		return ;
	// Dinamik düşünme süresi hesapla
	think_time = philo->data->time_to_die - philo->data->time_to_eat
		- philo->data->time_to_sleep;
	// Güvenli bölgeye çek (ölmeden önce biraz margin bırak)
	think_time = think_time / 2;
	// Çok kısa değilse düşün
	if (think_time > 1)
		usleep(think_time * 1000);
	else
		usleep(500); // Minimum 0.5ms
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
