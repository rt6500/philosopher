/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilis_thread_mutex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 08:37:41 by rseki             #+#    #+#             */
/*   Updated: 2025/04/26 08:37:42 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	handle_mutex_error(int status, t_opcode opcode)
{
	if (status == 0)
		return (0);
	printf("Raw error status: %d\n", status);
	if (status == EINVAL)
	{
		if (opcode == INIT)
			printf("Error\nInvalid attribute or mutex pointer\n");
		else
			printf("Error\nInvalid mutex (not initialized?)\n");
	}
	else if (status == EDEADLK)
		printf("Error\nA deadlock would occur\n");
	else if (status == EPERM)
		printf("Error\nThe current thread does not hold a lock on mutex\n");
	else if (status == ENOMEM)
		printf("Error\nNot enough memory to initialize mutex\n");
	else if (status == EBUSY)
		printf("Error\nMutex is still locked (cannot destroy it).\n");
	else
		printf("Error\nUnknown error (%d)\n", status);
	return (1);
}

int	handle_mutex(pthread_mutex_t *mutex, t_opcode opcode)
{
	int	status;

	status = 0;
	if (opcode == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (opcode == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (opcode == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (opcode == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
		return (printf("Error\nUnknown operation\n"), 1);
	return (handle_mutex_error(status, opcode));
}

static int	handle_thread_error(int status, t_opcode opcode)
{
	if (status == 0)
		return (0);
	if (status == EINVAL && opcode == CREATE)
		printf("Error\nInvalid attribute.\n");
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		printf("Error\nthread is not a joinable thread.\n");
	else if (status == ESRCH && (opcode == DETACH || opcode == JOIN))
		printf("Error\nNo thread with the ID thread could be found.\n");
	else if (status == EDEADLK && opcode == JOIN)
		printf("Error\nA deadlock was detected or \
			thread specifies the calling thread.\n");
	else if (status == EAGAIN && opcode == CREATE)
		printf("Error\nInsufficient resources to create another thread.\n");
	else if (status == EPERM && opcode == CREATE)
		printf("Error\nPermission denied for thread attributes\n");
	else
		printf("Error\nUnkown error (%d)\n", status);
	return (1);
}

int	handle_thread(pthread_t *thread, void *(*foo)(void *), \
	void *data, t_opcode opcode)
{
	int	status;

	status = 0;
	if (opcode == CREATE)
		status = pthread_create(thread, NULL, foo, data);
	else if (opcode == JOIN)
		status = pthread_join(*thread, NULL);
	else if (opcode == DETACH)
		status = pthread_detach(*thread);
	else
		return (printf("Error\nUnknown operation\n"), 1);
	return (handle_thread_error(status, opcode));
}

