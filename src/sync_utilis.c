/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utilis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:05:51 by rseki             #+#    #+#             */
/*   Updated: 2025/04/29 10:05:53 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
SPINLOCK until all threads get ready
*/
int	wait_all_threads(t_rules *rule)
{
	bool	val;

	while (1)
	{
		if (get_bool(&rule->rule_mutex, &rule->all_threads_ready, &val))
			return (1);
		if (val)
			break ;
		usleep(1);
	}
	return (0);
}

/*
monitor busy waits untill all threads are running
*/
int	all_threads_running(pthread_mutex_t *mutex, long *threads, \
	long num_philos, bool *result)
{
	if (handle_mutex(mutex, LOCK))
		return (1);
	*result = (*threads == num_philos);
	if (handle_mutex(mutex, UNLOCK))
		return (1);
	return (0);
}

// Increase threads running to snc with the moniter
int	increase_long(pthread_mutex_t *mutex, long *value)
{
	if (handle_mutex(mutex, LOCK))
		return (1);
	(*value)++;
	if (handle_mutex(mutex, UNLOCK))
		return (1);
	return (0);
}

/*
Make the system fair
1. if number of philos is even
	id:0, 2, 4, ... => wait briefly 
	=> allow ID:1, 3, 5, ... to grab forks first.
2. if the number of philos is odd
	id:1, 3, 5, ... think first.
*/
int	de_synchronize_philo(t_philo *philo)
{
	if (philo->rules->num_philos % 2 == 0)
	{
		if (philo->id % 2 == 0)
		{
			if (smart_sleep(30e3, philo->rules))
				return (1);
		}
	}
	else
	{
		if (philo->id % 2)
		{
			if (think(philo, true))
				return (1);
		}
	}
	return (0);
}
