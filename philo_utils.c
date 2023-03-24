/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:27:29 by ikhabour          #+#    #+#             */
/*   Updated: 2023/03/24 22:44:48 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	meals_completed(t_philo *philo)
{
	int	i;
	int	meals;

	i = 0;
	meals = 0;
	while (i < philo->list.philo_num)
	{
		pthread_mutex_lock(&philo[i].mtx_status);
		if (philo[i].meals_ate == philo->list.meals)
			meals++;
		pthread_mutex_unlock(&philo[i].mtx_status);
		i++;
	}
	if (meals == philo->list.philo_num)
		return (1);
	return (0);
}

int	philo_death(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->list.philo_num)
	{
		pthread_mutex_lock(&philo[i].mtx_status);
		if (get_time(philo[i].last_meal) > philo[i].list.time_to_die)
		{
			pthread_mutex_unlock(&philo[i].mtx_status);
			pthread_mutex_lock(philo[i].write);
			printf("%ld ms %d %s\n", get_time(philo[i].begin), philo[i].id,
				"has died");
			return (1);
		}
		pthread_mutex_unlock(&philo[i].mtx_status);
		i++;
	}
	return (0);
}

int	mutex_init(pthread_mutex_t *forks, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		if (pthread_mutex_init(&(forks[i]), NULL))
			return (1);
		i++;
	}
	return (0);
}

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->write);
	printf("%ld ms %d %s\n", get_time(philo->begin), philo->id, msg);
	pthread_mutex_unlock(philo->write);
}

int	ft_atoi(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i++] - '0';
		if (result > 2147483647 && sign == 1)
			return (-1);
		if (result < -2147483647 && sign == -1)
			return (-2);
	}
	return (result * sign);
}
