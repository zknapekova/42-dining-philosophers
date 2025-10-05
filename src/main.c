#include "philo.h"
#include <stdlib.h>
#include <unistd.h>


void	philo_activity(t_philo *philo, t_philo_status status)
{
	time_t	start;
	time_t	time;

	start = get_time();
	print_status_message(status, philo);
	if (status == SLEEP)
		time = philo->data->t_sleep;
	else if (status == EAT)
		time = philo->data->t_eat;
	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_lock);
		if (philo->data->stop)
			break;
		pthread_mutex_unlock(&philo->data->stop_lock);
		if (start + time >= get_time())
			break;
	}
}

void	philo_eating(t_philo *philo)
{
	if (philo->id % 2)
		usleep(100000 * philo->data->n_philos);

	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop)
		return ;
	pthread_mutex_unlock(&philo->data->stop_lock);

	pthread_mutex_lock(&philo->r_fork->mutex);
	print_status_message(TAKE_FORK, philo);

	if (philo->r_fork->status == FREE)
		philo->r_fork->status = USED;

	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop)
		return ;
	pthread_mutex_unlock(&philo->data->stop_lock);

	pthread_mutex_lock(&philo->l_fork->mutex);
	print_status_message(TAKE_FORK, philo);

	if (philo->l_fork->status == FREE)
		philo->l_fork->status = USED;

	philo_activity(philo, EAT);

	if (++philo->n_meal == philo->data->count_eat) //should this be moved to monitoring thread?
	{
		pthread_mutex_lock(&philo->data->stop_lock);
		philo->data->stop = 1;
		pthread_mutex_unlock(&philo->data->stop_lock);
	}

	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->last_meal_lock);

	philo->l_fork->status = FREE;
    pthread_mutex_unlock(&philo->l_fork->mutex);
    philo->r_fork->status = FREE;
    pthread_mutex_unlock(&philo->r_fork->mutex);
}

void* routine(void* arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->last_meal_lock);
	while (philo->data->stop == 0) //need to use mutex so this variable cannot be used directly in while condition
	{
		philo_eating(philo);
		pthread_mutex_lock(&philo->data->stop_lock);
		if (philo->data->stop)
			return (NULL);
		pthread_mutex_unlock(&philo->data->stop_lock);
		philo_activity(philo, SLEEP);
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data	*data;
	int		i;

	i = 0;
	data = malloc(sizeof(t_data));
	if (!data)
		return (print_err(MALLOC_ERROR), 1);
	if (validate(argc, argv, data))
		return (1);
	data->start_time = get_time();
	if (data->n_philos == 1)
		return (one_philosopher(&data->philos[0]), clean_up(data), 1);
	while (i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].th, NULL, &routine, &data->philos[i]) != 0)
			return (print_err(TH_CREATE_ERROR), 1);
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_join(data->philos[i].th, NULL) != 0)
			return (print_err(TH_JOIN_ERROR), 1);
		i++;
	}
	return (clean_up(data), printf("all good"), 0);
}