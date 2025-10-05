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

void	philo_dies_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_lock);
	if (get_time() - philo->last_meal_time >= philo->data->t_die)
	{
		pthread_mutex_lock(&philo->data->stop_lock);
		philo->data->stop = 1;
		pthread_mutex_unlock(&philo->data->stop_lock);
		print_status_message(DIE, philo);
	}
	pthread_mutex_unlock(&philo->last_meal_lock);
}
