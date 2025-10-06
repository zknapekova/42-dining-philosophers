#include "philo.h"
#include <unistd.h>
#include <stdlib.h>
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

void	clean_up(t_data *data)
{
	int	i;

	i = 0;
	if (data)
	{
		while (i < data->n_philos)
		{
			pthread_mutex_destroy(&data->philos[i].l_fork->mutex);
			pthread_mutex_destroy(&data->philos[i].r_fork->mutex);
			pthread_mutex_destroy(&data->philos[i].last_meal_lock);
			i++;
		}
		pthread_mutex_destroy(&data->stop_lock);
		free(data->philos);
		free(data->forks);
		free(data);
	}
}

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status_message(t_philo_status status, t_philo *philo)
{
	if (status == SLEEP)
		printf("%ld %d is sleeping\n", get_time() - philo->data->start_time, philo->id);
	if (status == EAT)
		printf("%ld %d is eating\n", get_time() - philo->data->start_time, philo->id);
	if (status == TAKE_FORK)
		printf("%ld %d has taken a fork\n", get_time() - philo->data->start_time, philo->id);
	if (status == THINK)
		printf("%ld %d is thinking\n", get_time() - philo->data->start_time, philo->id);
	if (status == DIE)
		printf("%ld %d died\n", get_time() - philo->data->start_time, philo->id);
}