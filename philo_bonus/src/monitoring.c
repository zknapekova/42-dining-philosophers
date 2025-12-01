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


int	philo_dies_check(t_philo *philo)
{
	if ((get_time() >= philo->last_meal_time + philo->data->t_die))
	{
		if (print_status_message(DIE, philo, get_time()))
			return (ERROR_STATUS);
		if (sem_wait(philo->data->sem_stop))
			return (print_err(SEM_WAIT_ERROR), ERROR_STATUS);
		philo->data->stop = 1;
		if (sem_post(philo->data->sem_stop))
			return (print_err(SEM_POST_ERROR), ERROR_STATUS);
		return (DIE_FROM_STARVATION_EXIT_STATUS);
	}
	return (0);
}

int	check_simulation_stop_fl(t_data *data)
{
	int	result;

	result = 0;
	if (sem_wait(data->sem_stop))
		return (print_err(SEM_WAIT_ERROR), 1);
	if (data->stop)
		result = DIE_FROM_STARVATION_EXIT_STATUS;
	if (sem_post(data->sem_stop))
		return (print_err(SEM_POST_ERROR), 1);
	return (result);
}

#include <stdio.h>
void	*philo_monitoring_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("start of monitoring routine for philo %d\n", philo->id);
	while (1)
	{
		if (philo_dies_check(philo))
			return (NULL);
		usleep(50);
	}
	return (NULL);
}


