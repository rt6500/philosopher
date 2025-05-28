/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_eat_utilis.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:08:00 by rseki             #+#    #+#             */
/*   Updated: 2025/05/26 12:08:01 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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

static int	start_eating(t_philo *philo)
{
	long	now;

	if (handle_mutex(&philo->philo_mutex, LOCK))
		return (1);
	now = gettime(MILLISECONDS);
	if ((now - philo->last_meal_time) > philo->rules->time_to_die)
	{
		handle_mutex(&philo->philo_mutex, UNLOCK);
		return (0);
	}
	philo->last_meal_time = now;
	handle_mutex(&philo->philo_mutex, UNLOCK);
	philo->total_meals++;
	if (write_status(EAT, philo, DEBUG_MODE))
		return (1);
	if (smart_sleep(philo->rules->time_to_eat * 1000, philo->rules))
		return (1);
	return (0);
}

static int	finish_eating(t_philo *philo)
{
	if (philo->rules->limit_meals > 0 \
		&& philo->total_meals == philo->rules->limit_meals)
	{
		if (set_bool(&philo->philo_mutex, &philo->full, true))
			return (1);
	}
	return (0);
}

static int	drop_forks(t_philo *philo)
{
	if (handle_mutex(&philo->first_fork->fork, UNLOCK))
		return (1);
	if (handle_mutex(&philo->second_fork->fork, UNLOCK))
		return (1);
	return (0);
}

int	try_eating(t_philo *philo)
{
	if (grab_forks(philo))
		return (1);
	if (start_eating(philo))
		return (1);
	if (finish_eating(philo))
		return (1);
	if (drop_forks(philo))
		return (1);
	return (0);
}
