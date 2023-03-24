/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:28:01 by ikhabour          #+#    #+#             */
/*   Updated: 2023/03/24 21:31:15 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_arguments(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (argc != 5 && argc != 6)
	{
		write(2, "Invalid Arguments\n", 18);
		return (1);
	}
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][0] == '-')
			{
				write(2, "Invalid Arguments\n", 18);
				return (1);
			}
			if (!ft_isdigit(argv[i][j]))
			{
				write(2, "Invalid Arguments\n", 18);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

time_t	get_time(time_t start)
{
	struct timeval	tv;
	time_t			ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) - start;
	return (ms);
}

void	time_sleep(int sleep_time)
{
	struct timeval	start;
	struct timeval	end;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(100);
		gettimeofday(&end, NULL);
		if (((end.tv_sec * 1000) + (end.tv_usec / 1000)) - ((start.tv_sec
					* 1000) + (start.tv_usec / 1000)) >= sleep_time)
			break ;
	}
}

void	philo_init(t_philo **philo_info, t_list args, time_t begin)
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*mtx_write;
	int				i;

	mtx_write = malloc(sizeof(pthread_mutex_t));
	forks = malloc(sizeof(pthread_mutex_t) * args.philo_num);
	*philo_info = malloc(sizeof(t_philo) * args.philo_num);
	if (pthread_mutex_init(mtx_write, NULL) != 0)
		write(2, "Init failiure\n", 14);
	mutex_init(forks, args.philo_num);
	i = 0;
	while (i < args.philo_num)
	{
		(*philo_info)[i].forks.right = &(forks[i]);
		(*philo_info)[i].forks.left = &(forks[(i + 1) % args.philo_num]);
		(*philo_info)[i].write = mtx_write;
		(*philo_info)[i].id = i + 1;
		(*philo_info)[i].meals_ate = 0;
		(*philo_info)[i].list = args;
		(*philo_info)[i].begin = begin;
		(*philo_info)[i].last_meal = begin;
		pthread_mutex_init(&(*philo_info)[i].mtx_status, NULL);
		i++;
	}
}

int	args_init(t_list *args, char **argv)
{
	args->philo_num = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->eat_time = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (args->philo_num <= 0 || args->time_to_die <= 0 || args->eat_time <= 0
		|| args->time_to_sleep <= 0)
	{
		write(2, "Invalid Arguments\n", 18);
		return (1);
	}
	if (argv[5])
	{
		args->meals = ft_atoi(argv[5]);
		if (args->meals <= 0)
		{
			write(2, "Invalid Arguments\n", 18);
			return (1);
		}
	}
	else
		args->meals = -1;
	return (0);
}