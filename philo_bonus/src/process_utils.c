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

int	init_process(t_data *data)
{
	int	i;
	int	status;
	int exit_status;
	
	i = 0;
	data->start_time = get_time() + (data->n_philos * 4);
	while (i < data->n_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].pid = fork();
		if (data->philos[i].pid == -1)
			return(print_err(PROC_CREATE_ERROR), clean_up(data), 1);
		else if (data->philos[i].pid == 0)
		{
			routine(&data->philos[i]);
			break;
		}
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{
		waitpid(data->philos[i].pid, &status, 0);
		if (WIFEXITED(status)) 
    		exit_status = WEXITSTATUS(status);
		i++;
	}
	return (0);
}

static int	handle_err_status(int err_status, t_philo *philo)
{
	if (pthread_detach(philo->th))
	{
		print_err(TH_JOIN_ERROR);
		return (clean_up(philo->data), ERROR_STATUS);
	}
	return (clean_up(philo->data), err_status);
}

void	routine(t_philo *philo)
{
	int	err;
	
	err = EXIT_SUCCESS;
	if (pthread_create(&philo->th, NULL, &philo_monitoring_routine, philo) != 0)
	{
		print_err(TH_CREATE_ERROR);
		exit (handle_err_status(ERROR_STATUS, philo));	
	}
	while (get_time() < philo->data->start_time)
		usleep(100);
	if (philo->id % 2)
		usleep(10 * philo->data->t_eat);
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

