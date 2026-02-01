/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:22 by zuknapek          #+#    #+#             */
/*   Updated: 2026/01/11 19:15:56 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <sys/wait.h>

void	wait_for_processes(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->n_philos)
	{
		waitpid(data->philos[i].pid, &status, 0);
		i++;
	}
}

void	clean_up(t_data *data)
{
	int	i;

	i = 0;
	sem_close(data->sem_write);
	sem_close(data->sem_forks);
	sem_close(data->sem_stop_parent);
	while (i < data->n_philos)
	{
		sem_close(data->philos[i].sem_last_meal);
		if (data->count_eat > 0)
			sem_close(data->philos[i].sem_meal_count);
		i++;
	}
	free(data->philos);
	free(data);
}

t_data	*validate(int argc, char *argv[])
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (print_err(MALLOC_ERROR), NULL);
	data->n_philos = ft_atoi_philos(argv[1]);
	data->t_die = ft_atoi_philos(argv[2]);
	data->t_eat = ft_atoi_philos(argv[3]);
	data->t_sleep = ft_atoi_philos(argv[4]);
	data->count_eat = -1;
	data->count_eat_n_philos = 0;
	data->n_forks = data->n_philos;
	if (argc == 6)
		data->count_eat = ft_atoi_philos(argv[5]);
	if (data->n_philos <= 0 || data->t_die < 0 || data->t_eat < 0 \
|| data->t_sleep < 0 || data->count_eat == -2)
		return (print_err(NUM_ARG_ERROR), free(data), NULL);
	if (data->count_eat == 0)
		return (free(data), NULL);
	if (init_semaphors(data) || init_philos(data))
		return (clean_up(data), NULL);
	return (data);
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
		return (clean_up(data), err);
	if (pthread_create(&data->monitoring_th, NULL, \
&parent_monitoring_routine, data) != 0)
		return (clean_up(data), print_err(TH_CREATE_ERROR), 1);
	wait_for_processes(data);
	if (sem_post(data->sem_stop_parent))
		return (print_err(SEM_POST_ERROR), clean_up(data), 1);
	if (pthread_join(data->monitoring_th, NULL))
		return (print_err(TH_JOIN_ERROR), clean_up(data), 1);
	return (clean_up(data), 0);
}
