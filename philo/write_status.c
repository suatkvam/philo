/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:49:42 by akivam            #+#    #+#             */
/*   Updated: 2025/11/18 14:44:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

void	write_status(t_philo *philo, const char *status)
{
	long long	timestamp;
	int			is_dead;

	timestamp = get_current_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->write_lock);
	pthread_mutex_lock(&philo->data->data_lock);
	is_dead = philo->data->is_dead;
	pthread_mutex_unlock(&philo->data->data_lock);
	if (is_dead == 0)
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	else if (status[0] == 'd')
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
}
