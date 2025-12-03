/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:08 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:52:58 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <signal.h>

int	philo_dies_check(t_philo *philo)
{
	if (sem_wait(philo->sem_last_meal))
		return (print_err(SEM_WAIT_ERROR), ERROR_STATUS);
	if ((get_time() >= philo->last_meal_time + philo->data->t_die))
	{
		if (print_status_message(DIE, philo, get_time()))
			return (ERROR_STATUS);
		if (sem_post(philo->data->sem_stop_parent))
			return (print_err(SEM_POST_ERROR), ERROR_STATUS);
		return (DIE_FROM_STARVATION_EXIT_STATUS);
	}
	if (sem_post(philo->sem_last_meal))
		return (print_err(SEM_POST_ERROR), ERROR_STATUS);
	return (0);
}

void	*philo_monitoring_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo_dies_check(philo))
			return (NULL);
		usleep(70);
	}
	return (NULL);
}

void	*parent_monitoring_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	if (sem_wait(data->sem_stop_parent))
		return (print_err(SEM_WAIT_ERROR), NULL);
	i = 0;
	while (i < data->n_philos)
	{
		kill(data->philos[i].pid, SIGTERM);
		i++;
	}
	return (NULL);
}
