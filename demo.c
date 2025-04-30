#include <pthread.h>
#include <stdio.h>

// Global Variables
pthread_mutex_t lock; // a mutex object.
int		shared_counter = 0;

/// pthread_create demand a function which return void*. it can return a pointer or NULL.
// (void *) = a generic pointer. pass anything to the thread.
void	*increment(void *arg)
{
	int	i;

	// Inside the thread,
	// you cast it to the actural type you need.
	i = 0;
	while (i < 100000)
	{
		pthread_mutex_lock(&lock);
		shared_counter++;
		pthread_mutex_unlock(&lock);
		i++;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	t2;

	pthread_t t1; // an identifier for the thread created.
	pthread_mutex_init(&lock, NULL);
	// 2nd argument is for mutex attributes. NULL=default.
	pthread_create(&t1, NULL, increment, NULL); //(&id, thread attributes,
	//	function, argument passed to the funciton)
	pthread_create(&t2, NULL, increment, NULL);
	pthread_join(t1, NULL); // main() now waits (is blocked).
							// the next line runs ONLY after thread A is done
	pthread_join(t2, NULL); // main() now waits (is blocked).
							// the next line runs ONLY after thread A is done
	printf("final count: %d\n", shared_counter);
	pthread_mutex_destroy(&lock);
	return (0);
}
