/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:17 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:53:10 by zuknapek         ###   ########.fr       */
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

int	update_last_meal_time(t_philo *philo)
{
	if (sem_wait(philo->sem_last_meal))
		return (print_err(SEM_WAIT_ERROR), ERROR_STATUS);
	philo->last_meal_time = get_time();
	if (sem_post(philo->sem_last_meal))
		return (print_err(SEM_POST_ERROR), ERROR_STATUS);
	return (0);
}

int	get_activity_time(t_philo *philo, t_philo_status status, time_t *time)
{
	if (status == SLEEP)
		*time = philo->data->t_sleep;
	else if (status == EAT)
	{
		*time = philo->data->t_eat;
		return (philo_dies_check(philo));

	}
	else if (status == THINK)
		*time = determine_think_time(philo);
	return (0);
}


void	philo_activity(t_philo *philo, t_philo_status status, int *err)
{
	time_t	start;
	time_t	time;

	*err = get_activity_time(philo, status, &time);
	if (*err != 0)
		return ;
	start = get_time();
	*err = print_status_message(status, philo, start);
	if (*err != 0)
		return ;
	if (status == EAT)
	{
		*err = update_last_meal_time(philo);
		if (*err != 0)
			return ;
	}
	while (1)
	{
		if (start + time <= get_time())
			break ;
	}
}

void	philo_eating(t_philo *philo, int *err)
{
	if (sem_wait(philo->data->sem_forks))
	{
		print_err(SEM_WAIT_ERROR);
		*err = ERROR_STATUS;
		return ;
	}
	*err = print_status_message(TAKE_FORK, philo, get_time());
	if (*err)
		return ;
	if (sem_wait(philo->data->sem_forks))
	{
		print_err(SEM_WAIT_ERROR);
		*err = ERROR_STATUS;
		return ;
	}
	*err = print_status_message(TAKE_FORK, philo, get_time());
	if (*err)
		return ;
	philo_activity(philo, EAT, err);
	if (*err)
		return ;
	if (sem_post(philo->data->sem_forks))
	{
		print_err(SEM_POST_ERROR);
		*err = ERROR_STATUS;
		return ;
	}
	if (sem_post(philo->data->sem_forks))
	{
		print_err(SEM_POST_ERROR);
		*err = ERROR_STATUS;
		return ;
	}
	if (++philo->n_meal == philo->data->count_eat)
	{
		*err = MEAL_LIMIT_REACHED_EXIT_STATUS;
		return ;
	}
}

