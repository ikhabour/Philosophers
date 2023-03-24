/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 22:51:09 by ikhabour          #+#    #+#             */
/*   Updated: 2023/03/24 21:18:32 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->write);
	printf("%ld ms %d %s\n", get_time(philo->begin), philo->id, msg);
	pthread_mutex_unlock(philo->write);
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
		pthread_create(&(philo_info->philos), NULL, routine, &(philo_info[i]));
		usleep(100);
		i++;
	}
}

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

void	mutex_init(pthread_mutex_t *forks, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		if (pthread_mutex_init(&(forks[i]), NULL))
			write(2, "Init faliure\n", 14);
		i++;
	}
}

void	philo_init(t_philo **philo_info, t_list args, time_t begin)
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*mtx_write = malloc(sizeof(pthread_mutex_t));
	int				i;

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

time_t	get_time(time_t start)
{
	struct timeval	tv;
	time_t			ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) - start;
	return (ms);
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

int	meals_completed(t_philo *philo)
{
	int	i;
	int meals;

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
	philo_init(&philo_info, args, begin);
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
	// pthread_exit(0);
}
