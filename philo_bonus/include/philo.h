/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:47 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:38:02 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>

# define N_PHILOS_MAX 250

# define MALLOC_ERROR "Malloc failed"
# define TH_CREATE_ERROR "Failed to create thread"
# define TH_JOIN_ERROR "Failed to join thread"
# define N_ARG_ERROR "Incorrect number of arguments"
# define NUM_ARG_ERROR "Incorrect numeric argument"
# define SEM_INIT_ERROR "Semafor initialization and opening failed"
# define SEM_WAIT_ERROR "Semafor lock failed"
# define SEM_POST_ERROR "Semafor unlock failed"
# define PROC_CREATE_ERROR "Creation of a child process failed"

struct	s_data;


typedef enum e_philo_status
{
	SLEEP,
	EAT,
	THINK,
	TAKE_FORK,
	DIE
}	t_philo_status;


typedef struct s_philo
{
	int				id;
	pthread_t		th;
	int				n_meal;
	struct s_data	*data;
	time_t			last_meal_time;
	int				pid;
	char			*sem_last_meal_name;
	sem_t			*sem_last_meal;
}	t_philo;

typedef struct s_data
{
	int				n_philos;
	int				n_forks;
	time_t			t_die;
	time_t			t_sleep;
	time_t			t_eat;
	int				count_eat;
	int				count_eat_n_philos;
	int				stop;
	time_t			start_time;
	t_philo			*philos;
	sem_t			*sem_stop;
	sem_t			*sem_write;
	sem_t			*sem_forks;
	pthread_t		monitoring_th;
}	t_data;

int		ft_strlen(char *str);
void	print_err(char *message);
int		ft_atoi_philos(const char *nptr);
t_data	*validate(int argc, char *argv[]);
void	clean_up(t_data *data);
time_t	get_time(void);
void	one_philosopher(t_philo *philo);
int		philo_dies_check(t_philo *philo);
int		check_simulation_stop_fl(t_data *data);
int		start_monitor(t_data *data);
int		philo_activity(t_philo *philo, t_philo_status status);
time_t	determine_think_time(t_philo *philo);
void	update_last_meal_time(t_philo *philo);
int		philo_eating(t_philo *philo);
int		print_status_message(t_philo_status status, \
	t_philo *philo, time_t start_time);
int		create_threads(t_data *data);
int		join_threads(t_data *data);
void	detach_threads(t_data *data, int n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);

#endif
