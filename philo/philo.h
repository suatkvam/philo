#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

struct	s_philo;

typedef struct s_philo_data
{
	long long		number_of_philosophers;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;

	long long		start_time;
	int				is_dead;
	int				ready_philos_count;
	struct s_philo	*philos;

	pthread_mutex_t	start_gate_lock;
	pthread_mutex_t	*forks;

	pthread_mutex_t	data_lock;
	pthread_mutex_t	write_lock;

}					t_philo_data;

typedef struct s_philo
{
	pthread_t		thread_id;
	int				id;
	long long		eat_count;
	long long		last_eat_time;
	long long		must_eat_target;

	t_philo_data	*data;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

}					t_philo;

int					validate_arguments(int argc, char const *argv[]);
int					parse_data(int argc, char const *argv[],
						t_philo_data *philo_data, long long *must_eat_value);
int					allocate_resources(t_philo_data *philo_data);
void				init_mutexes_and_philos(t_philo_data *philo_data,
						long long must_eat_value);
void				start_simulation(t_philo_data *philo_data);
void				observer(t_philo_data *philo_data);
void				clean_resources(t_philo_data *data);
void				write_status(t_philo *philo, const char *status);
void				*philo_routine(void *argument);

#endif