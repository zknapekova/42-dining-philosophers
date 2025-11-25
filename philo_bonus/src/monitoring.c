/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:08 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:52:58 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

void	*monitoring_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->n_philos)
		{
			if (philo_dies_check(&data->philos[i]))
				return (NULL);
			i++;
		}
		usleep(50);
	}
	return (NULL);
}

int	start_monitor(t_data *data)
{
	if (pthread_create(&data->monitoring_th, NULL, &monitoring_routine, \
		data) != 0)
		return (print_err(TH_CREATE_ERROR), 1);
	return (0);
}

int	philo_dies_check(t_philo *philo)
{
	int	status;

	status = 0;
	pthread_mutex_lock(&philo->last_meal_lock);
	if ((get_time() >= philo->last_meal_time + philo->data->t_die || \
	philo->data->n_philos == philo->data->count_eat_n_philos))
	{
		if (philo->data->count_eat_n_philos != philo->data->n_philos)
			print_status_message(DIE, philo, get_time());
		pthread_mutex_lock(&philo->data->stop_lock);
		philo->data->stop = 1;
		pthread_mutex_unlock(&philo->data->stop_lock);
		status = 1;
	}
	pthread_mutex_unlock(&philo->last_meal_lock);
	return (status);
}

int	check_simulation_stop_fl(t_data *data)
{
	int	result;

	result = 0;
	if (sem_wait(philo->data->sem_stop))
		return (print_err(SEM_WAIT_ERROR), 1);
	if (data->stop)
		result = 1;
	if (sem_post(philo->data->sem_stop))
		return (print_err(SEM_POST_ERROR), 1);
	return (result);
}

int	one_philosopher(t_philo *philo)
{
	while (get_time() < philo->data->start_time)
		usleep(100);
	philo->pid = fork()
	if (philo->pid == 0)
		return(print_err(PROC_CREATE_ERROR), clean_up(data), 1);
	print_status_message(TAKE_FORK, philo, get_time());
	usleep(1000 * philo->data->t_die);
	print_status_message(DIE, philo, get_time());
	if (sem_wait(philo->data->sem_stop))
			return (print_err(SEM_WAIT_ERROR), 1);
	philo->data->stop = 1;
	if (sem_post(philo->data->sem_stop))
			return (print_err(SEM_POST_ERROR), 1);
}
