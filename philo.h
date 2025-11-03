#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

struct s_philo;

typedef struct s_philo_data
{
	long long		number_of_philosophers;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;

	int				is_dead;

	struct s_philo	*philos;

	pthread_mutex_t	*forks;

	pthread_mutex_t	data_lock;
	pthread_mutex_t	write_lock;

}					t_philo_data;

typedef struct s_philo
{
	int				id;
	long long		eat_count;
	long long		last_eat_time;
	long long		must_eat_target;

	t_philo_data	*data;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

}					t_philo;

#endif