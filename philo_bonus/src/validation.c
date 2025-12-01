/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:33:13 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:33:13 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <fcntl.h>


static int	init_data(int argc, char *argv[], t_data *data)
{
	data->n_philos = ft_atoi_philos(argv[1]);
	data->t_die = ft_atoi_philos(argv[2]);
	data->t_eat = ft_atoi_philos(argv[3]);
	data->t_sleep = ft_atoi_philos(argv[4]);
	data->count_eat = -1;
	data->count_eat_n_philos = 0;
	data->stop = 0;
	data->n_forks = data->n_philos;
	if (argc == 6)
		data->count_eat = ft_atoi_philos(argv[5]);
	return (0);
}


static int	init_philos(t_data *data)
{
	int		i;
	char	*temp;
	char	*sem_last_meal_name;

	i = -1;
	temp = NULL;
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (print_err(MALLOC_ERROR), 1);
	while (++i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].n_meal = 0;
		data->philos[i].data = data;
		temp = ft_itoa(data->philos[i].id);
		sem_last_meal_name = ft_strjoin("/sem_last_meal", temp);
		if (!temp ||!sem_last_meal_name)
			return(print_err(MALLOC_ERROR), 1);
		sem_unlink(sem_last_meal_name);
		data->philos[i].sem_last_meal = sem_open(sem_last_meal_name, O_CREAT, 0644, 1);
		sem_unlink(sem_last_meal_name);
		if (data->philos[i].sem_last_meal == SEM_FAILED)
			return (free(temp), free(sem_last_meal_name), print_err(SEM_INIT_ERROR), 1);
		free(sem_last_meal_name);
		free(temp);
	}
	return (0);
}

static int	init_semaphors(t_data *data)
{
	sem_unlink("/sem_stop");
	sem_unlink("/sem_write");
	sem_unlink("/sem_forks");
	data->sem_stop = sem_open("/sem_stop", O_CREAT, 0644, 1);
	sem_unlink("/sem_stop");
	if (data->sem_stop == SEM_FAILED)
		return (print_err(SEM_INIT_ERROR), 1);
	data->sem_write = sem_open("/sem_write", O_CREAT, 0644, 1);
	sem_unlink("/sem_write");
	if (data->sem_write == SEM_FAILED)
		return (print_err(SEM_INIT_ERROR), 1);
	data->sem_forks = sem_open("/sem_forks", O_CREAT, 0644, data->n_philos);
	sem_unlink("/sem_forks");
	if (data->sem_forks == SEM_FAILED)
		return (print_err(SEM_INIT_ERROR), 1);
	return (0);
}


t_data	*validate(int argc, char *argv[])
{
	t_data	*data;
	
	data = malloc(sizeof(t_data));
	if (!data)
		return (print_err(MALLOC_ERROR), NULL);
	if (init_data(argc, argv, data))
		return (free(data), NULL);
	if (data->n_philos <= 0 || data->t_die < 0 || data->t_eat < 0 \
	|| data->t_sleep < 0 || data->count_eat == -2)
		return (print_err(NUM_ARG_ERROR), free(data), NULL);
	if (data->count_eat == 0)
		return (free(data), NULL);
	if (init_semaphors(data) || init_philos(data))
		return (clean_up(data), NULL);
	return (data);
}
