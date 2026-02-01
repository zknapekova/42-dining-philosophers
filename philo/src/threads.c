/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:33:55 by zuknapek          #+#    #+#             */
/*   Updated: 2026/01/07 18:34:38 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (get_time() < philo->data->start_time)
		usleep(100);
	if (philo->id % 2 && philo->data->n_philos < 50)
		usleep(50 * philo->data->n_philos);
	else if (philo->id % 2 && philo->data->n_philos >= 50)
		usleep(50 * philo->data->n_philos / 2);
	while (1)
	{
		if (philo_eating(philo) || \
philo_activity(philo, SLEEP) || \
philo_activity(philo, THINK))
			return (NULL);
	}
	return (NULL);
}

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].th, NULL, &routine, \
&data->philos[i]) != 0)
			return (print_err(TH_CREATE_ERROR), detach_threads(data, i - 1), 1);
		i++;
	}
	return (0);
}

int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_join(data->philos[i].th, NULL) != 0)
			return (print_err(TH_JOIN_ERROR), 1);
		i++;
	}
	if (pthread_join(data->monitoring_th, NULL) != 0)
		return (print_err(TH_JOIN_ERROR), 1);
	return (0);
}

void	detach_threads(t_data *data, int n)
{
	int	i;

	i = 0;
	while (i <= n)
	{
		pthread_detach(data->philos[i].th);
		i++;
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
			if (i == 0)
				pthread_mutex_destroy(&data->philos[i].l_fork->mutex);
			else
				pthread_mutex_destroy(&data->philos[i].r_fork->mutex);
			pthread_mutex_destroy(&data->philos[i].last_meal_lock);
			i++;
		}
		pthread_mutex_destroy(&data->stop_lock);
		pthread_mutex_destroy(&data->write_lock);
		free(data->philos);
		free(data->forks);
		free(data);
	}
}
