# 42-dining-philosophers

The project implements the Dining Philosophers problem solution. In the first part, each philosopher is represented as a thread and mutexes are used to manage the access to forks. In the bonus part, all forks are placed in the middle of the table, each philosopher is represented as a process, and the number of available forks is controlled by a semaphore.

Input arguments:

◦ number_of_philosophers: The number of philosophers and also the number
of forks.

◦ time_to_die (in milliseconds): If a philosopher has not started eating within
time_to_die milliseconds since the start of their last meal or the start of the
simulation, they die.

◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.

◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.

◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.
