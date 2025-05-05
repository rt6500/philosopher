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
void	wait_all_threads(t_rules *rule)
{
	bool val;

	while (1)
	{
		if (!get_bool(&rule->rule_mutex, &rule->all_threads_ready, &val))
			break ;
		if (val)
			break ;
		usleep(1);
	}
}

/*
monitor busy waits untill all threads are running
*/

bool	all_threads_running(pthread_mutex_t *mutex, long *threads,
		long num_philos)
{
	bool	ret;

	ret = false;
	handle_mutex(mutex, LOCK);
	if (*threads == num_philos)
		ret = true;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}

// Increase threads running to snc with the moniter
void	increase_long(pthread_mutex_t *mutex, long *value)
{
	handle_mutex(mutex, LOCK);
	(*value)++;
	handle_mutex(mutex, UNLOCK);
}

/*
Make the system fair
*/

void	de_synchronize_philo(t_philo *philo)
{
	if (philo->rules->num_philos % 2 == 0)
	{
		if (philo->id % 2 == 0)
			smart_sleep(30e3, philo->rules);
	}
	else
	{
		if (philo->id % 2)
			think(philo, true);
	}
}