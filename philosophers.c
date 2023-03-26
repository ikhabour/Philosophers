/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 22:51:09 by ikhabour          #+#    #+#             */
/*   Updated: 2023/03/26 01:18:45 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->forks.right);
	print_msg(philo, "has taken a fork");
	pthread_mutex_lock((philo->forks.left));
	print_msg(philo, "has taken a fork");
	print_msg(philo, "is eating");
	pthread_mutex_lock(&philo->mtx_status);
	philo->last_meal = get_time(0);
	philo->meals_ate++;
	pthread_mutex_unlock(&philo->mtx_status);
	time_sleep(philo->list.eat_time);
	pthread_mutex_unlock((philo->forks.right));
	pthread_mutex_unlock((philo->forks.left));
}

void	*routine(void *philo_info)
{
	t_philo	*philo;

	philo = (t_philo *)philo_info;
	while (1)
	{
		eating(philo);
		print_msg(philo, "is sleeping");
		time_sleep(philo->list.time_to_sleep);
		print_msg(philo, "is thinking");
	}
	return (NULL);
}

void	create_philosophers(int philo_num, t_philo *philo_info)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_create(&(philo_info[i].philos), NULL, routine, &(philo_info[i]));
		usleep(100);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_list	args;
	t_philo	*philo_info;
	int		i;
	time_t	begin;

	i = 0;
	if (check_arguments(argc, argv))
		return (1);
	if (args_init(&args, argv))
		return (1);
	begin = get_time(0);
	if (philo_init(&philo_info, args, begin))
		return (1);
	create_philosophers(ft_atoi(argv[1]), philo_info);
	while (i < ft_atoi(argv[1]))
		pthread_detach(philo_info[i++].philos);
	while (1)
	{
		if (philo_death(philo_info))
			break ;
		if (meals_completed(philo_info))
			break ;
	}
}
