/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:33:13 by zuknapek          #+#    #+#             */
/*   Updated: 2025/12/07 16:27:59 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <fcntl.h>

static char	*get_sem_name(int id, char *base_name)
{
	char	*temp;
	char	*sem_name;

	temp = ft_itoa(id);
	if (!temp)
		return (print_err(MALLOC_ERROR), NULL);
	sem_name = ft_strjoin(base_name, temp);
	free(temp);
	if (!sem_name)
		return (print_err(MALLOC_ERROR), NULL);
	return (sem_name);
}

int	init_sem_meal_count(t_philo *philo)
{
	char	*sem_meal_count_name;

	if (philo->data->count_eat > 0)
	{
		sem_meal_count_name = get_sem_name(philo->id, \
			SEM_MEAL_COUNT_NAME);
		if (!sem_meal_count_name)
			return (print_err(MALLOC_ERROR), 1);
		sem_unlink(sem_meal_count_name);
		philo->sem_meal_count = sem_open(sem_meal_count_name, \
			O_CREAT, 0644, 1);
		sem_unlink(sem_meal_count_name);
		if (philo->sem_meal_count == SEM_FAILED)
			return (print_err(SEM_INIT_ERROR), free(sem_meal_count_name), 1);
		free(sem_meal_count_name);
	}
	return (0);
}

int	init_sem_last_meal(t_philo *philo)
{
	char	*sem_last_meal_name;

	sem_last_meal_name = get_sem_name(philo->id, SEM_LAST_MEAL_NAME);
	if (!sem_last_meal_name)
		return (print_err(MALLOC_ERROR), 1);
	sem_unlink(sem_last_meal_name);
	philo->sem_last_meal = sem_open(sem_last_meal_name, O_CREAT, \
		0644, 1);
	sem_unlink(sem_last_meal_name);
	if (philo->sem_last_meal == SEM_FAILED)
		return (print_err(SEM_INIT_ERROR), free(sem_last_meal_name), 1);
	free(sem_last_meal_name);
	return (0);
}

int	init_philos(t_data *data)
{
	int		i;

	i = -1;
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (print_err(MALLOC_ERROR), 1);
	while (++i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].n_meal = 0;
		data->philos[i].data = data;
		data->philos[i].pid = -1;
		if (init_sem_last_meal(&data->philos[i]))
			return (1);
		if (init_sem_meal_count(&data->philos[i]))
			return (1);
	}
	return (0);
}

int	init_semaphors(t_data *data)
{
	sem_unlink("/sem_write");
	sem_unlink("/sem_forks");
	sem_unlink("/sem_stop_parent");
	data->sem_write = sem_open("/sem_write", O_CREAT, 0644, 1);
	sem_unlink("/sem_write");
	if (data->sem_write == SEM_FAILED)
		return (print_err(SEM_INIT_ERROR), 1);
	data->sem_forks = sem_open("/sem_forks", O_CREAT, 0644, data->n_philos);
	sem_unlink("/sem_forks");
	if (data->sem_forks == SEM_FAILED)
		return (print_err(SEM_INIT_ERROR), 1);
	data->sem_stop_parent = sem_open("/sem_stop_parent", O_CREAT, 0644, 1);
	sem_unlink("/sem_stop_parent");
	if (data->sem_stop_parent == SEM_FAILED)
		return (print_err(SEM_INIT_ERROR), 1);
	if (sem_post(data->sem_stop_parent))
		return (print_err(SEM_POST_ERROR), 1);
	return (0);
}
