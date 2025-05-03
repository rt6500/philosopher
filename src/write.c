/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:29:12 by rseki             #+#    #+#             */
/*   Updated: 2025/05/01 11:29:14 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
timestamp_in_ms X has taken a fork
[time_ms] [philo_id] [action]
*/

static void	write_status_debug(t_philo_status status, t_philo *philo,
		long elapsed)
{
	if ((status == TAKE_FIRST_FORK) && !simulation_finished(philo->rules))
		printf("%-6ld %d has taken a 1st fork of %d\n", elapsed, philo->id,
			philo->first_fork->fork_id);
	if ((status == TAKE_SECOND_FORK) && !simulation_finished(philo->rules))
		printf("%-6ld %d has taken a 2nd fork of %d\n", elapsed, philo->id,
			philo->second_fork->fork_id);
	else if ((status == EAT) && !simulation_finished(philo->rules))
		printf("%-6ld %d is eating. total meals : %ld\n", elapsed, philo->id,
			philo->total_meals);
	else if ((status == SLEEP) && !simulation_finished(philo->rules))
		printf("%-6ld %d is sleeping\n", elapsed, philo->id);
	else if ((status == THINK) && !simulation_finished(philo->rules))
		printf("%-6ld %d is thinking\n", elapsed, philo->id);
	else if ((status == DIED) && !simulation_finished(philo->rules))
		printf("%-6ld %d died\n", elapsed, philo->id);
}

void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long elapsed;

	// printf("philo %d: entering write_status\n", philo->id);
	elapsed = gettime(MILLISECONDS) - philo->rules->start_time;
	if (philo->full)
		return ;
	handle_mutex(&philo->rules->write_lock, LOCK);
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
	{
		if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
			&& !simulation_finished(philo->rules))
			printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
		else if ((status == EAT) && !simulation_finished(philo->rules))
			printf("%-6ld %d is eating\n", elapsed, philo->id);
		else if ((status == SLEEP) && !simulation_finished(philo->rules))
			printf("%-6ld %d is sleeping\n", elapsed, philo->id);
		else if ((status == THINK) && !simulation_finished(philo->rules))
			printf("%-6ld %d is thinking\n", elapsed, philo->id);
		else if ((status == DIED) && !simulation_finished(philo->rules))
			printf("%-6ld %d died\n", elapsed, philo->id);
	}
	handle_mutex(&philo->rules->write_lock, UNLOCK);
}