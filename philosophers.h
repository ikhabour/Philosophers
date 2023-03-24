/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 22:50:44 by ikhabour          #+#    #+#             */
/*   Updated: 2023/03/24 23:31:15 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <errno.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_forks
{
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
}					t_forks;

typedef struct s_list
{
	int				philo_num;
	int				eat_time;
	int				time_to_die;
	int				time_to_sleep;
	int				meals;
}					t_list;

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*write;
	pthread_mutex_t	mtx_status;
	t_list			list;
	pthread_t		philos;
	int				meals_ate;
	time_t			begin;
	t_forks			forks;
	time_t			last_meal;
}					t_philo;

int					ft_atoi(const char *str);
int					check_arguments(int argc, char **argv);
time_t				get_time(time_t start);
void				time_sleep(int sleep_time);
int					meals_completed(t_philo *philo);
int					philo_death(t_philo *philo);
int					mutex_init(pthread_mutex_t *forks, int philo_num);
int					philo_init(t_philo **philo_info, t_list args, time_t begin);
int					args_init(t_list *args, char **argv);
int					ft_isdigit(int c);
void				eating(t_philo *philo);
void				create_philosophers(int philo_num, t_philo *philo_info);
void				print_msg(t_philo *philo, char *msg);
int					print_error(char *msg, int len, int err);
int					ft_strlen(char *str);
void				init_vars(t_philo **philo_info, t_list args, time_t begin);
#endif