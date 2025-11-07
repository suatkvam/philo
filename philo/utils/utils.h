#ifndef UTILS_H
# define UTILS_H

# include <sys/time.h>

# define LLONG_MAX 9223372036854775807

// 1second = 100 millisecond
#define SEC_TO_MS 1000

// 1 millisecond = 100 microsecond
#define USEC_TO_MS 1000
int			is_not_numeric(const char *str);

long long	ft_atoll(const char *str);
long long	get_current_time(void);

#endif