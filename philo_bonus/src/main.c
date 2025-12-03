/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:22 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:38:34 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
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
		{
    		exit_status = WEXITSTATUS(status);
			printf("init_process philo %d exited with status %d \n", data->philos[i].id, exit_status);
		}
		i++;
	}
	
	return (0);
}

void	clean_up(t_data *data)
{
	int	i;
	
	i = 0;
	sem_close(data->sem_write);
	sem_close(data->sem_forks);
	sem_close(data->sem_stop_parent);
	while (i < data->n_philos)
		sem_close(data->philos[i++].sem_last_meal);
	free(data->philos);
	free(data);
}


int	main(int argc, char *argv[])
{
	t_data	*data;
	int		err;

	if (argc < 5 || argc > 6)
		return (print_err(N_ARG_ERROR), 1);
	data = validate(argc, argv);
	if (!data)
		return (1);
	if (pthread_create(&data->monitoring_th, NULL, \
	&parent_monitoring_routine, data) != 0)
		return (clean_up(data), print_err(TH_CREATE_ERROR), 1);
	err = init_process(data);
	if (err)
		return (clean_up(data), pthread_detach(data->monitoring_th), err);
	pthread_detach(data->monitoring_th);
	return (clean_up(data), 0);
}
