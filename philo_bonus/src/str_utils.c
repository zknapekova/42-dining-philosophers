#include "philo.h"

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

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (s1 && !s2)
		return ((char *)s1);
	if (!s1 && s2)
		return ((char *)s2);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	res = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, s1_len + 1);
	ft_strlcat(res, s2, s1_len + s2_len + 1);
	return (res);
}

static int	ft_get_count_int_itoa(int n)
{
	int	count;

	count = 0;
	if (!n)
		return (1);
	if (n < 0)
		count++;
	while (n)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char		*result;
	int			len;
	long int	n_long;
	int			i;

	len = ft_get_count_int_itoa(n);
	i = len - 1;
	n_long = (long) n;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	if (n_long < 0)
	{
		result[0] = '-';
		n_long = n_long * (-1);
	}
	if (!n_long)
		result[0] = '0';
	while (n_long)
	{
		result[i--] = n_long % 10 + '0';
		n_long /= 10;
	}
	result[len] = '\0';
	return (result);
}


