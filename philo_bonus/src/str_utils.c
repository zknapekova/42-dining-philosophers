#include "philo.h"
#include <stdlib.h>

int	ft_strlen(const char *str)
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


