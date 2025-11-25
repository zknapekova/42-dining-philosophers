/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuknapek <zuknapek@student.42prague.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:34:22 by zuknapek          #+#    #+#             */
/*   Updated: 2025/10/12 16:38:34 by zuknapek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

int	main(int argc, char *argv[])
{
	t_data	*data;

	
	if (argc < 5 || argc > 6)
		return (print_err(N_ARG_ERROR), 1);
	data = validate(argc, argv);
	if (!data)
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
