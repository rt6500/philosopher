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
thinking routine
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
	t_think = t_eat * 2- t_sleep;
	if (t_think < 0)
		t_think = 0;
	if (smart_sleep(t_think * 0.42, philo->rules))
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
	bool	finished;

	philo = (t_philo *)arg;
	if (wait_all_threads(philo->rules))
		return ((void *)1);
	if (set_long(&philo->philo_mutex, &philo->last_meal_time, \
		gettime(MILLISECONDS)))
		return ((void *)1);
	if (increase_long(&philo->rules->rule_mutex, \
		&philo->rules->threads_running_nbr))
		return ((void *)1);
	if (write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE))
		return ((void *)1);
	while (1)
	{
		if (simulation_finished(philo->rules, &finished))
			return ((void *)1);
		if (finished)
			break ;
		usleep(1);
	}
	return ((void *)0);
}

static int	grab_forks(t_philo *philo)
{
	if (handle_mutex(&philo->first_fork->fork, LOCK))
		return (1);
	if (write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE))
		return (1);
	if (handle_mutex(&philo->second_fork->fork, LOCK))
		return (1);
	if (write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE))
		return (1);
	return (0);
}

/*
eat routine
1) grab the forks: 1st and 2nd.abort
2) update write eat, last_meal_time, total_meals,
3) release the forks
*/
int	eat(t_philo *philo)
{
	if (philo->full)
		return (0);
	else
	{
		if (grab_forks(philo))
			return (1);
		if (set_long(&philo->philo_mutex, &philo->last_meal_time, \
			gettime(MILLISECONDS)))
			return (1);
		philo->total_meals++;
		if (write_status(EAT, philo, DEBUG_MODE))
			return (1);
		if (smart_sleep(philo->rules->time_to_eat, philo->rules))
			return (1);
		if (philo->rules->limit_meals > 0
			&& philo->total_meals == philo->rules->limit_meals)
		{
			if (set_bool(&philo->philo_mutex, &philo->full, true))
				return (1);
		}
		if (handle_mutex(&philo->first_fork->fork, UNLOCK))
			return (1);
		if (handle_mutex(&philo->second_fork->fork, UNLOCK))
			return (1);
	}
	return (0);
}
