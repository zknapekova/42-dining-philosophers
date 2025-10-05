# ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>

# define N_PHILOS_MAX 250;

# define MALLOC_ERROR "Malloc failed"
# define TH_CREATE_ERROR "Failed to create thread"
# define TH_JOIN_ERROR "Failed to join thread"
# define N_ARG_ERROR "Incorrect number of arguments"
# define NUM_ARG_ERROR "Incorrect numeric argument"
# define MUTEX_INIT_ERROR "Initiating mutex failed"

struct s_data;

typedef enum e_fork_status {
  FREE,
  USED,
} t_fork_status;

typedef enum e_philo_status {
  SLEEP,
  EAT,
  THINK,
  TAKE_FORK,
  DIE
} t_philo_status;

typedef struct s_fork
{
	t_fork_status	status;
	int				id;
	pthread_mutex_t	mutex;
} t_fork;

typedef struct s_philo
{
	int				id;
	pthread_t 		th;
	int				n_meal;
	t_fork			*l_fork;
	t_fork			*r_fork;
	struct s_data	*data;
	time_t			last_meal_time;
	pthread_mutex_t	last_meal_lock;
}	t_philo;

typedef struct s_data
{
	int				n_philos;
	int				n_forks;
	time_t			t_die;
	time_t			t_sleep;
	time_t			t_eat;
	int				count_eat;
	int				stop;
	time_t			start_time;
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	stop_lock;
	pthread_t 		monitoring_th;
}	t_data;


int		ft_strlen(char *str);
void	print_err(char *message);
int		ft_atoi_philos(const char *nptr);
int		validate(int argc, char *argv[], t_data *data);
void	clean_up(t_data *data);
time_t	get_time(void);
void	print_status_message(t_philo_status status, t_philo *philo);
void	one_philosopher(t_philo *philo);
void	philo_dies(t_philo *philo);


# endif