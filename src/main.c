#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

void* routine(void* arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!check_simulation_stop_fl(philo->data))
	{
		philo_eating(philo);
		if (check_simulation_stop_fl(philo->data))
			return (NULL);
		if (philo_activity(philo, SLEEP))
			return (NULL);
		if (philo_activity(philo, THINK))
			return (NULL);
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
	if (data->n_philos == 1)
		return (one_philosopher(&data->philos[0]), clean_up(data), 1);
	data->start_time = get_time();
	while (i < data->n_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].th, NULL, &routine, &data->philos[i]) != 0)
			return (print_err(TH_CREATE_ERROR), 1);
		i++;
	}
	if (start_monitor(data))
		return (print_err(TH_CREATE_ERROR), 1);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_join(data->philos[i].th, NULL) != 0)
			return (print_err(TH_JOIN_ERROR), 1);
		i++;
	}
	if (pthread_join(data->monitoring_th, NULL) != 0)
		return (print_err(TH_JOIN_ERROR), 1);
	return (clean_up(data), 0);
}