/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:17 by zuknapek          #+#    #+#             */
/*   Updated: 2026/01/07 18:32:03 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

time_t	determine_think_time(t_philo *philo)
{
	time_t	time_to_think;
	time_t	q;

	q = philo->data->t_die - (philo->data->t_eat * 2 + philo->data->t_sleep);
	if (philo->data->n_philos % 2)
	{
		time_to_think = (philo->data->t_eat * 2
				- philo->data->t_sleep);
		if (q > 0 && q < 30)
			time_to_think = time_to_think * q / philo->data->t_die;
	}
	else
	{
		time_to_think = (philo->data->t_eat
				- philo->data->t_sleep);
		if (q > 0 && q < 30)
			time_to_think = time_to_think * q / philo->data->t_die;
	}
	if (time_to_think < 0)
		time_to_think = 0;
	return (time_to_think);
}

void	update_last_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->last_meal_lock);
}

int	get_activity_time(t_philo *philo, t_philo_status status, time_t *time)
{
	if (status == SLEEP)
		*time = philo->data->t_sleep;
	else if (status == EAT)
	{
		*time = philo->data->t_eat;
		if (philo_dies_check(philo))
			return (1);
	}
	else if (status == THINK)
		*time = determine_think_time(philo);
	return (0);
}

int	philo_activity(t_philo *philo, t_philo_status status)
{
	time_t	start;
	time_t	time;

	if (get_activity_time(philo, status, &time))
		return (1);
	start = get_time();
	if (print_status_message(status, philo, start))
		return (1);
	if (status == EAT)
		update_last_meal_time(philo);
	while (1)
	{
		if (check_simulation_stop_fl(philo->data))
			return (1);
		if (start + time <= get_time())
			return (0);
		usleep(100);
	}
	return (0);
}

int	philo_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->r_fork->mutex);
	philo->r_fork->status = UNAVAILABLE;
	if (print_status_message(TAKE_FORK, philo, get_time()))
		return (pthread_mutex_unlock(&philo->r_fork->mutex), 1);
	pthread_mutex_lock(&philo->l_fork->mutex);
	philo->l_fork->status = UNAVAILABLE;
	if (print_status_message(TAKE_FORK, philo, get_time()))
		return (pthread_mutex_unlock(&philo->r_fork->mutex), \
pthread_mutex_unlock(&philo->l_fork->mutex), 1);
	if (philo_activity(philo, EAT))
		return (pthread_mutex_unlock(&philo->r_fork->mutex), \
pthread_mutex_unlock(&philo->l_fork->mutex), 1);
	pthread_mutex_lock(&philo->last_meal_lock);
	if (++philo->n_meal == philo->data->count_eat && \
philo->data->count_eat_n_philos < philo->data->n_philos)
		philo->data->count_eat_n_philos++;
	pthread_mutex_unlock(&philo->last_meal_lock);
	philo_dies_check(philo);
	philo->l_fork->status = AVAILABLE;
	philo->r_fork->status = AVAILABLE;
	pthread_mutex_unlock(&philo->l_fork->mutex);
	pthread_mutex_unlock(&philo->r_fork->mutex);
	return (0);
}
