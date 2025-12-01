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


int	init_process(t_data *data)
{
	int	i;
	int	status;
	int	exit_pid;
	
	i = 0;
	data->start_time = get_time() + (data->n_philos * 4);
	while (i <= data->n_philos)
	{
		data->philos[i].pid=fork();
		if (data->philos[i].pid == -1)
			return(print_err(PROC_CREATE_ERROR), clean_up(data), 1);
		else if (data->philos[i].pid == 0)
			routine(&data->philos[i]);
		i++;
	}

	exit_pid = wait(&status);
	if (WIFEXITED(status))
	{
        printf("Exit status: %d\n", WEXITSTATUS(status));
        i = 0;
        while (i <= data->n_philos)
        {
        	if (data->philos[i].pid != exit_pid)
        		kill(data->philos[i].pid, SIGTERM);
        	i++;
        }
        i = 0;
        while (i <= data->n_philos)
        {
        	waitpid(data->philos[i].pid, NULL, 0);
        	i++;
        }
    }
	return (0);
	//create global monitoring thread
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
