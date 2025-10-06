#include "philo.h"
#include <unistd.h>

void	one_philosopher(t_philo *philo)
{
	pthread_mutex_lock(&philo->r_fork->mutex);
	print_status_message(TAKE_FORK, philo);
	usleep(1000 * philo->data->t_die);
	pthread_mutex_lock(&philo->data->stop_lock);
	philo->data->stop = 1;
	pthread_mutex_unlock(&philo->data->stop_lock);
	print_status_message(DIE, philo);
	pthread_mutex_unlock(&philo->r_fork->mutex);
}


time_t	determine_think_time(t_philo *philo)
{
	time_t	time_to_think;

	time_to_think = philo->data->t_die - (philo->data->t_sleep + philo->data->t_eat);
	if (time_to_think < 0)
		time_to_think = 0;
	return time_to_think;
}

void	update_last_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->last_meal_lock);
}

int	philo_activity(t_philo *philo, t_philo_status status)
{
	time_t	start;
	time_t	time;

	start = get_time();
	print_status_message(status, philo);
	if (status == SLEEP)
		time = philo->data->t_sleep;
	else if (status == EAT)
	{
		time = philo->data->t_eat;
		update_last_meal_time(philo);
	}
	else if (status == THINK)
		time = determine_think_time(philo);
	while (1)
	{
		if (check_simulation_stop_fl(philo->data))
			return (1);
		if (start + time <= get_time())
			return (0);
	}
	return (0);
}


int	philo_eating(t_philo *philo)
{
	if (philo->id % 2)
		usleep(100 * philo->data->n_philos);
	pthread_mutex_lock(&philo->r_fork->mutex);
	if (check_simulation_stop_fl(philo->data))
		return (pthread_mutex_unlock(&philo->r_fork->mutex), 0);
	print_status_message(TAKE_FORK, philo);
	philo->r_fork->status = USED;
	pthread_mutex_lock(&philo->l_fork->mutex);
	if (check_simulation_stop_fl(philo->data))
		return (pthread_mutex_unlock(&philo->r_fork->mutex), \
		pthread_mutex_unlock(&philo->l_fork->mutex), 0);
	print_status_message(TAKE_FORK, philo);
	philo->l_fork->status = USED;
	if (philo_activity(philo, EAT))
		return (pthread_mutex_unlock(&philo->r_fork->mutex), \
		pthread_mutex_unlock(&philo->l_fork->mutex), 0);
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->n_meal++;
	pthread_mutex_unlock(&philo->last_meal_lock);
	philo_dies_check(philo);
	philo->l_fork->status = FREE;
    pthread_mutex_unlock(&philo->l_fork->mutex);
    philo->r_fork->status = FREE;
    pthread_mutex_unlock(&philo->r_fork->mutex);
    return (1);
}