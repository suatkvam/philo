#include "philo.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	*philo_routine(void *argument);
// arguman check
int	validate_arguments(int argc, char const *argv[])
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (is_not_numeric(argv[i]) == 1)
		{
			printf("Error: Invalid argument format \n");
			return (1);
		}
		i++;
	}
	return (0);
}
// data parse
int	parse_data(int argc, char const *argv[], t_philo_data *philo_data,
		long long *must_eat_value)
{
	philo_data->number_of_philosophers = ft_atoll(argv[1]);
	philo_data->time_to_die = ft_atoll(argv[2]);
	philo_data->time_to_eat = ft_atoll(argv[3]);
	philo_data->time_to_sleep = ft_atoll(argv[4]);
	philo_data->is_dead = 0;
	*must_eat_value = -1;
	if (argc == 6)
		*must_eat_value = ft_atoll(argv[5]);
	if (philo_data->number_of_philosophers <= 0 || philo_data->time_to_die <= 0
		|| philo_data->time_to_eat <= 0 || philo_data->time_to_sleep <= 0
		|| (argc == 6 && *must_eat_value <= 0))
	{
		printf("Error: Invalid argument value (must be > 0 and no overflow)\n");
		return (1);
	}
	return (0);
}
// allocation for philos and forks
int	allocate_resources(t_philo_data *philo_data)
{
	philo_data->philos = malloc(sizeof(t_philo)
			* philo_data->number_of_philosophers);
	philo_data->forks = malloc(sizeof(pthread_mutex_t)
			* philo_data->number_of_philosophers);
	if (!philo_data->philos || !philo_data->forks)
	{
		printf("Error: Malloc faild \n");
		free(philo_data->philos);
		free(philo_data->forks);
		return (1);
	}
	return (0);
}
// filo and mutex initilaziton
void	init_mutexes_and_philos(t_philo_data *philo_data,
		long long must_eat_value)
{
	int	i;
	int	n;

	pthread_mutex_init(&philo_data->data_lock, NULL);
	pthread_mutex_init(&philo_data->write_lock, NULL);
	pthread_mutex_init(&philo_data->start_gate_lock, NULL);
	pthread_mutex_lock(&philo_data->start_gate_lock);
	philo_data->ready_philos_count = 0;
	i = -1;
	n = philo_data->number_of_philosophers;
	while (++i < n)
	{
		philo_data->philos[i].id = i + 1;
		philo_data->philos[i].eat_count = 0;
		philo_data->philos[i].must_eat_target = must_eat_value;
		philo_data->philos[i].data = philo_data;
		pthread_mutex_init(&philo_data->forks[i], NULL);
		if (n == 1)
		{
			philo_data->philos[i].left_fork = &philo_data->forks[i];
			philo_data->philos[i].right_fork = NULL;
		}
		else if (philo_data->philos[i].id % 2 == 0)
		{
			// Önce sol çatalı alacak (ID ile aynı index'teki)
			philo_data->philos[i].left_fork = &philo_data->forks[i];
			// Sonra sağ çatalı alacak (bir sonraki)
			// (i + 1) % n -> son filozofun çatalının 0 olmasını sağlar
			philo_data->philos[i].right_fork = &philo_data->forks[(i + 1) % n];
		} // Tek ID'li filozoflar (örn: 1, 3, 5)
		else
		{
			// Önce sağ çatalı alacak (ters sıralama)
			philo_data->philos[i].left_fork = &philo_data->forks[(i + 1) % n];
			// Sonra sol çatalı alacak
			philo_data->philos[i].right_fork = &philo_data->forks[i];
		}
	}
}

void	eat_routine(t_philo *philo)
{
	// Çatal ataması sayesinde (Adım 2), bu sıralama
	// artık kilitlenmeye (deadlock) yol açmaz.
	pthread_mutex_lock(philo->left_fork);
	write_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	write_status(philo, "has taken a fork");
	// yemek yeme
	write_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->data_lock);
	philo->last_eat_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	usleep(philo->data->time_to_eat * 1000);
	// çatalları bırak
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
void	sleep_routine(t_philo *philo)
{
	write_status(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

void	think_routine(t_philo *philo)
{
	write_status(philo, "is thinking");
}
void	start_simulation(t_philo_data *philo_data)
{
	int	i;

	// create philo threads
	i = -1;
	while (++i < philo_data->number_of_philosophers)
		pthread_create(&philo_data->philos[i].thread_id, NULL, philo_routine,
			&philo_data->philos[i]);
	// wait for say everyone "ready"(spin wait)
	while (1)
	{
		pthread_mutex_lock(&philo_data->data_lock);
		if (philo_data->ready_philos_count == philo_data->number_of_philosophers)
		{
			// if everyone ready break loop
			pthread_mutex_unlock(&philo_data->data_lock);
			break ;
		}
		pthread_mutex_unlock(&philo_data->data_lock);
		usleep(100); // cpu'yu yakmamak için kısa bi bekleme
	}
	// if everyone ready start_simulation
	philo_data->start_time = get_current_time();
	// last eat time is begining
	i = -1;
	while (++i < philo_data->number_of_philosophers)
		philo_data->philos[i].last_eat_time = philo_data->start_time;
	// "start" signal: open the gate
	pthread_mutex_unlock(&philo_data->start_gate_lock);
}
// philo routine(for each philos) logic
void	*philo_routine(void *argument)
{
	t_philo	*philo;

	philo = (t_philo *)argument;
	// 1. is philo ready
	pthread_mutex_lock(&philo->data->data_lock);
	philo->data->ready_philos_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	// start main thread
	//    Kapı zaten kilitli olduğu için (Adım 2'de kilitledik),
	//    bu fonksiyon burada donup bekleyecek.
	pthread_mutex_lock(&philo->data->start_gate_lock);
	// 3. Kapı açıldı! Kapıdan geç ve hemen serbest bırak ki
	//    diğer filozoflar da geçebilsin.
	pthread_mutex_unlock(&philo->data->start_gate_lock);
	// 1 philo dururmu
	if (philo->data->number_of_philosophers == 1)
	{
		write_status(philo, "has taken a fork");
		usleep(philo->data->time_to_die * 1000 + 1000);
		return (NULL);
	}
	while (philo->data->is_dead == 0)
	{
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
		// kodla dedik marsa kaçıp intahar ettiler
	}
	return (NULL);
}

void	write_status(t_philo *philo, const char *status)
{
	long long	timestamp;

	// Mesajların çakışmaması için yazma kilidini al
	pthread_mutex_lock(&philo->data->write_lock);
	// Simülasyon başından beri geçen zamanı hesapla
	timestamp = get_current_time() - philo->data->start_time;
	// Sadece simülasyon devam ediyorsa (veya bu bir ölüm mesajıysa) yazdır
	if (philo->data->is_dead == 0)
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	else if (status[0] == 'd') // "died" mesajı için özel durum
	{
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	// Yazma kilidini bırak
	pthread_mutex_unlock(&philo->data->write_lock);
}

void	observer(t_philo_data *philo_data)
{
	int	i;
	int	full_philo_cnt;

	while (philo_data->is_dead == 0)
	{
		full_philo_cnt = 0;
		i = -1;
		while (++i < philo_data->number_of_philosophers)
		{
			// data race önlemek için kilitliyorun
			pthread_mutex_lock(&philo_data->data_lock);
			// philo öldü mü kontrolü
			if (get_current_time()
				- philo_data->philos[i].last_eat_time > philo_data->time_to_die)
			{
				// öldü simülasyonu durdur
				philo_data->is_dead = 1;
				// kilidi aç
				pthread_mutex_unlock(&philo_data->data_lock);
				// ölüm mesajını yazdır
				write_status(&philo_data->philos[i], "died"); // bu uazılacak
				return ;
			}
			// philo doydu mu(optina)
			if (philo_data->philos[i].must_eat_target != -1
				&& philo_data->philos[i].eat_count >= philo_data->philos[i].must_eat_target)
				full_philo_cnt++;
			pthread_mutex_unlock(&philo_data->data_lock);
			// kontrol edilenin kilidini aç
		}
		// tün filolar doydu mu
		// eğer hedef varsa ve "doyan" sayısı toplam sayıya eşitse
		if (philo_data->philos[0].must_eat_target != -1
			&& full_philo_cnt == philo_data->number_of_philosophers)
		{
			// simülasyonu durdur
			pthread_mutex_lock(&philo_data->data_lock);
			philo_data->is_dead = 1;
			pthread_mutex_unlock(&philo_data->data_lock);
			return ;
		}
		// obseerver sleep for not to burn cpu
		usleep(1000);
	}
	return ;
}

void	clean_resources(t_philo_data *data)
{
	int	i;
	int	n;

	i = -1;
	n = data->number_of_philosophers;
	while (++i < n)
	{
		pthread_join(data->philos[i].thread_id, NULL);
	}
	i = -1;
	while ((++i < n))
	{
		pthread_mutex_destroy(&data->forks[i]);
	}
	pthread_mutex_destroy(&data->data_lock);
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->start_gate_lock);
	// 3. Hafızayı serbest bırak
	free(data->philos);
	free(data->forks);
}

int	main(int argc, char const *argv[])
{
	t_philo_data	philo_data;
	long long		must_eat_value;

	if (validate_arguments(argc, argv) != 0)
		return (1);
	if (parse_data(argc, argv, &philo_data, &must_eat_value) != 0)
		return (1);
	if (allocate_resources(&philo_data) != 0)
		return (1);
	init_mutexes_and_philos(&philo_data, must_eat_value);
	start_simulation(&philo_data);
	observer(&philo_data);
	clean_resources(&philo_data);
	return (0);
}
