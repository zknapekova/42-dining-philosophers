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
	int	exit_pid;
	
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
	exit_pid = wait(&status);
	usleep(1500);
	printf("exit pid: %d\n", exit_pid);
	usleep(1500);
	i = 0;
	while (i < data->n_philos)
	{
		if (data->philos[i].pid != exit_pid)
		{
			printf("philo %d to be killed\n", data->philos[i].id);
			kill(data->philos[i].pid, SIGTERM);
		}
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{
		printf("loop2: philo %d\n", data->philos[i].id);
		waitpid(data->philos[i].pid, NULL, 0);
		i++;
	}
	return (0);
	//create global monitoring thread
}

void	clean_up(t_data *data)
{
	sem_close(data->sem_stop);
	sem_close(data->sem_write);
	sem_close(data->sem_forks);
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
	err = init_process(data);
	if (err)
		return (err);
	/*if (create_threads(data))
		return (clean_up(data), 1);
	if (start_monitor(data))
		return (detach_threads(data, data->n_philos), clean_up(data), 1);*/
	return (clean_up(data), 0);
}
