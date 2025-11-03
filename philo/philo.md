Global rules
You have to write a program for the mandatory part and another one for the bonus part
(if you decide to do the bonus part). They both have to comply with the following rules:
* Global variables are forbidden!
* Your program(s) must take the following arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]

> **Example Usage:**
> 
> `./philo 4 800 200 200`
> 
> `./philo 5 800 200 200 7`

* `number_of_philosophers`: The number of philosophers and also the number
    of forks.
* `time_to_die` (in milliseconds): If a philosopher has not started eating within
    time_to_die milliseconds since the start of their last meal or the start of the
    simulation, they die.
* `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat.
    During that time, they will need to hold two forks.
* `time_to_sleep` (in milliseconds): The time a philosopher will spend sleeping.
* `number_of_times_each_philosopher_must_eat` (optional argument): If all
    philosophers have eaten at least number_of_times_each_philosopher_must_eat
    times, the simulation stops. If not specified, the simulation stops when a
    philosopher dies.
* Each philosopher has a number ranging from 1 to number_of_philosophers.
* Philosopher number 1 sits next to philosopher number number_of_philosophers.
    Any other philosopher, numbered N, sits between philosopher N - 1 and philosopher
    N + 1.

8
Philosophers I never thought philosophy would be so deadly
About the logs of your program:
* Any state change of a philosopher must be formatted as follows:
    -   `timestamp_in_ms X has taken a fork`
    -   `timestamp_in_ms X is eating`
    -   `timestamp_in_ms X is sleeping`
    -   `timestamp_in_ms X is thinking`
    -   `timestamp_in_ms X died`
    Replace `timestamp_in_ms` with the current timestamp in milliseconds
    and `X` with the philosopher number.
* A displayed state message should not overlap with another message.
* A message announcing a philosopher’s death must be displayed within 10 ms of
    their actual death.
* Again, philosophers should avoid dying!

> **IMPORTANT:**
> # Your program must not have any data races.

philo mandatory specific rules

* Each philosopher must be represented as a separate thread.
* There is one fork between each pair of philosophers. Therefore, if there are several
    philosophers, each philosopher has a fork on their left side and a fork on their right
    side. If there is only one philosopher, they will have access to just one fork.
* To prevent philosophers from duplicating forks, you should protect each fork’s state
    with a mutex.
