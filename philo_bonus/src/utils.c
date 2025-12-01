/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:33:37 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:43:03 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>


void	print_err(char *message)
{
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
}

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	print_status_message(t_philo_status status, t_philo *philo, \
	time_t start_time)
{
	int	err;
	
	err = 0;
	if (sem_wait(philo->data->sem_write))
		return (print_err(SEM_WAIT_ERROR), ERROR_STATUS);
	err = check_simulation_stop_fl(philo->data);
	if (err)
	{
		printf("philo %d exit check\n", philo->id);
		return err;
	}
	if (status == SLEEP)
		printf("%ld %d is sleeping\n", start_time - \
			philo->data->start_time, philo->id);
	if (status == EAT)
		printf("%ld %d is eating\n", start_time - \
			philo->data->start_time, philo->id);
	if (status == TAKE_FORK)
		printf("%ld %d has taken a fork\n", start_time - \
			philo->data->start_time, philo->id);
	if (status == THINK)
		printf("%ld %d is thinking\n", start_time - \
			philo->data->start_time, philo->id);
	if (status == DIE)
		printf("%ld %d died\n", get_time() - \
		philo->data->start_time, philo->id);
	else
		if (sem_post(philo->data->sem_write))
			return (print_err(SEM_POST_ERROR), ERROR_STATUS);
	return (0);
}

