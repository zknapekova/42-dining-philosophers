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
	if (pthread_mutex_init(&data->stop_lock, NULL) != 0)
		return (print_err(MUTEX_INIT_ERROR), 1);
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
		return (print_err(MUTEX_INIT_ERROR), 1);
	return (0);
}

static int	init_mutexes(t_data *data, int i)
{
	if (pthread_mutex_init(&data->philos[i].r_fork->mutex, NULL) != 0)
		return (print_err(MUTEX_INIT_ERROR), 1);
	if (pthread_mutex_init(&data->philos[i].l_fork->mutex, NULL) != 0)
		return (print_err(MUTEX_INIT_ERROR), 1);
	if (pthread_mutex_init(&data->philos[i].last_meal_lock, NULL) != 0)
		return (print_err(MUTEX_INIT_ERROR), 1);
	return (0);
}

static int	init_philos(t_data *data)
{
	int	i;

	i = -1;
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (print_err(MALLOC_ERROR), 1);
	while (++i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].n_meal = 0;
		data->philos[i].data = data;
		if (i != 0)
		{
			data->philos[i].r_fork = &data->forks[i];
			data->philos[i].l_fork = &data->forks[i - 1];
		}
		else
		{
			data->philos[i].r_fork = &data->forks[data->n_philos - 1];
			data->philos[i].l_fork = &data->forks[i];
		}
		if (init_mutexes(data, i))
			return (1);
	}
	return (0);
}

static int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(t_fork) * data->n_philos);
	if (!data->forks)
		return (print_err(MALLOC_ERROR), 1);
	while (i < data->n_philos)
	{
		data->forks[i].id = i + 1;
		data->forks[i].status = AVAILABLE;
		i++;
	}
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
	if (init_forks(data) || init_philos(data))
		return (clean_up(data), NULL);
	return (data);
}
