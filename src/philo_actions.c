/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:28:39 by rseki             #+#    #+#             */
/*   Updated: 2025/05/06 13:28:41 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
thinking routine for improving the survival rate
if the number of philos is odds, it's better to control thinking time.
To prevent too thinking in a row.
*/
int	think(t_philo *philo, bool pre_simulation)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_simulation)
	{
		if (write_status(THINK, philo, DEBUG_MODE))
			return (1);
	}
	if (philo->rules->num_philos % 2 == 0)
		return (0);
	t_eat = philo->rules->time_to_eat;
	t_sleep = philo->rules->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	if (smart_sleep(t_think * 0.42 * 1e3, philo->rules))
		return (1);
	return (0);
}

/*
for only one philo
1)fake lock to the fork.
2)sleep untill the monitor will bust it
*/
void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (wait_all_threads(philo->rules))
		return (NULL);
	if (set_long(&philo->philo_mutex, &philo->last_meal_time, \
		gettime(MILLISECONDS)))
		return (NULL);
	if (increase_long(&philo->rules->rule_mutex, \
		&philo->rules->threads_running_nbr))
		return (NULL);
	if (write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE))
		return (NULL);
	usleep(philo->rules->time_to_die * 1e3);
	if (write_status(DIED, philo, DEBUG_MODE))
		return (NULL);
	return (philo);
}

int	eat(t_philo *philo)
{
	bool	died;

	if (philo->full)
		return (0);
	if (philo_died(philo, &died))
		return (1);
	if (died)
		return (0);
	return (try_eating(philo));
}
