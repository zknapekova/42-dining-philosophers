#include "philo.h"
#include <stdlib.h>


int	main(int argc, char *argv[])
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (print_err(MALLOC_ERROR), 1);
	if (validate(argc, argv, data))
		return (1);
	data->start_time = get_time() + (data->n_philos * 4);
	if (data->n_philos == 1)
		return (one_philosopher(&data->philos[0]), clean_up(data), 1);
	if (create_threads(data))
		return (clean_up(data), 1);
	if (start_monitor(data))
		return (detach_threads(data, data->n_philos), clean_up(data), 1);
	if (join_threads(data) == 1)
		return (clean_up(data), 1);
	return (clean_up(data), 0);
}