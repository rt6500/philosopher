#include "../philo.h"

void	error_exit(const char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

void	how_long(t_rules rule)
{
	struct timeval	now;
	long			elapsed_ms;

	gettimeofday(&now, NULL);
	elapsed_ms = (now.tv_sec - rule.start_time.tv_sec) * 1e3 + (now.tv_usec
			- rule.start_time.tv_usec) / 1e3;
	printf("elapsed time: %ld ms\n", elapsed_ms);
}

void	free_array_philo(t_philo **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_array_fork(t_fork **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	*malloc_with_check(size_t size)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
		return (write(1, "Error\n", 6), NULL);
	else
		return (p);
}

