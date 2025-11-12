/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:20:57 by akivam            #+#    #+#             */
/*   Updated: 2025/11/12 21:20:58 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdlib.h>

long long	get_current_time(void)
{
	struct timeval	timeval;
	long long		time_in_mili_second;

	if (gettimeofday(&timeval, NULL) != 0)
		return (0);
	time_in_mili_second = (timeval.tv_sec * SEC_TO_MS) + (timeval.tv_usec
			/ USEC_TO_MS);
	return (time_in_mili_second);
}
