/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:33:37 by zuknapek          #+#    #+#             */
/*   Updated: 2026/01/07 18:37:34 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <sys/time.h>

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	if (str)
	{
		while (str[len])
			len++;
	}
	return (len);
}

void	print_err(char *message)
{
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
}

int	ft_atoi_philos(const char *nptr)
{
	int	res;

	res = 0;
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if ((*nptr < 48 || *nptr > 57) && (*nptr != 43))
		return (-2);
	if (*nptr == 43)
		nptr++;
	while (*nptr >= 48 && *nptr <= 57)
	{
		res *= 10;
		res += *nptr - '0';
		nptr++;
	}
	if (*nptr && (*nptr < 48 || *nptr > 57) && (*nptr != 43))
		return (-2);
	return (res);
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
	pthread_mutex_lock(&philo->data->write_lock);
	if (check_simulation_stop_fl(philo->data))
		return (pthread_mutex_unlock(&philo->data->write_lock), 1);
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
	{
		printf("%ld %d died\n", get_time() - \
philo->data->start_time, philo->id);
		pthread_mutex_lock(&philo->data->stop_lock);
		philo->data->stop = 1;
		pthread_mutex_unlock(&philo->data->stop_lock);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
	return (0);
}
