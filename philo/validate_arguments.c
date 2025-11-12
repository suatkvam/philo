/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:21:30 by akivam            #+#    #+#             */
/*   Updated: 2025/11/12 21:21:31 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils/utils.h"

int	validate_arguments(int argc, char const *argv[])
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (is_not_numeric(argv[i]) == 1)
		{
			printf("Error: Invalid argument format \n");
			return (1);
		}
		i++;
	}
	return (0);
}
