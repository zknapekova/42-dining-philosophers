/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:33:55 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:33:55 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


static int	handle_err_status(int err_status, t_philo *philo)
{
	if (pthread_detach(philo->th) != 0)
		return (print_err(TH_JOIN_ERROR), clean_up(philo->data), ERROR_STATUS);
	return (clean_up(philo->data), err_status);
}


void	routine(t_philo *philo)
{
	int	err;
	
	err = EXIT_SUCCESS;
	philo->last_meal_time = philo->data->start_time;
	sem_unlink(SEM_LAST_MEAL_NAME);
	philo->sem_last_meal = sem_open(SEM_LAST_MEAL_NAME, O_CREAT, 0644, 1);
	sem_unlink(SEM_LAST_MEAL_NAME);
	if (philo->sem_last_meal == SEM_FAILED)
	{	
		print_err(SEM_INIT_ERROR);
		exit (handle_err_status(ERROR_STATUS, philo));	
	}
	if (pthread_create(&philo->th, NULL, &philo_monitoring_routine, philo) != 0)
	{
		print_err(TH_CREATE_ERROR);
		exit (handle_err_status(ERROR_STATUS, philo));	
	}
	while (get_time() < philo->data->start_time)
		usleep(100);
	if (philo->id % 2 && philo->data->n_philos < 50)
		usleep(50 * philo->data->n_philos);
	else if (philo->id % 2 && philo->data->n_philos >= 50)
		usleep(50 * philo->data->n_philos / 2);
	while (1)
	{
		philo_eating(philo, &err);
		if (err)
			exit(handle_err_status(err, philo));
		philo_activity(philo, SLEEP, &err);
		if (err)
			exit(handle_err_status(err, philo));
		philo_activity(philo, THINK, &err);
		if (err)
			exit(handle_err_status(err, philo));
	}
}


void	clean_up(t_data *data)
{
	int	i;

	i = 0;
	if (data)
	{
		while (i < data->n_philos)
		{
			if (data->philos[i].sem_last_meal)
				sem_close(data->philos[i].sem_last_meal);
			i++;
		}
		if (data->sem_stop)
			sem_close(data->sem_stop);
		if (data->sem_write)
			sem_close(data->sem_write);
		if (data->sem_forks)
			sem_close(data->sem_forks);
		free(data->philos);
		free(data);
	}
}
