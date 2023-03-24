/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 22:32:16 by ikhabour          #+#    #+#             */
/*   Updated: 2023/03/24 23:32:17 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_error(char *msg, int len, int err)
{
	write(2, msg, len);
	return (err);
}

void	init_vars(t_philo **philo_info, t_list args, time_t begin)
{
	int	i;

	i = 0;
	while (i < args.philo_num)
	{
		(*philo_info)[i].id = i + 1;
		(*philo_info)[i].meals_ate = 0;
		(*philo_info)[i].list = args;
		(*philo_info)[i].begin = begin;
		(*philo_info)[i].last_meal = begin;
		i++;
	}
}
