#include "philo.h"


void* monitoring_routine(void* arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	i = 0;
	while (1)
	{
		if
	}
	return (NULL);
}



int	start_monitor(t_data *data)
{
	if (pthread_create(&data->monitoring_th, NULL, &monitoring_routine, &data) != 0)
			return (print_err(TH_CREATE_ERROR), 1);
}