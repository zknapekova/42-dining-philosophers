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
	if (sem_wait(philo->data->sem_write))
		return (print_err(SEM_WAIT_ERROR), ERROR_STATUS);
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

static int	ft_get_count_int_itoa(int n)
{
	int	count;

	count = 0;
	if (!n)
		return (1);
	if (n < 0)
		count++;
	while (n)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char		*result;
	int			len;
	long int	n_long;
	int			i;

	len = ft_get_count_int_itoa(n);
	i = len - 1;
	n_long = (long) n;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	if (n_long < 0)
	{
		result[0] = '-';
		n_long = n_long * (-1);
	}
	if (!n_long)
		result[0] = '0';
	while (n_long)
	{
		result[i--] = n_long % 10 + '0';
		n_long /= 10;
	}
	result[len] = '\0';
	return (result);
}

