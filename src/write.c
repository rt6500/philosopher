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
static void	write_status_debug(t_philo_status status, t_philo *philo, \
		long elapsed)
{
	bool	finished;

	simulation_finished(philo->rules, &finished);
	if ((status == TAKE_FIRST_FORK) && !finished)
		printf("%-6ld %d has taken a 1st fork of %d\n", elapsed, philo->id, \
			philo->first_fork->fork_id);
	if ((status == TAKE_SECOND_FORK) && !finished)
		printf("%-6ld %d has taken a 2nd fork of %d\n", elapsed, philo->id, \
			philo->second_fork->fork_id);
	else if ((status == EAT) && !finished)
		printf("%-6ld %d is eating. total meals : %ld\n", elapsed, philo->id, \
			philo->total_meals);
	else if ((status == SLEEP) && !finished)
		printf("%-6ld %d is sleeping\n", elapsed, philo->id);
	else if ((status == THINK) && !finished)
		printf("%-6ld %d is thinking\n", elapsed, philo->id);
	else if ((status == DIED) && !finished)
		printf("%-6ld %d died\n", elapsed, philo->id);
}

static void	print_status_line(t_philo_status status, t_philo *philo, \
	long elapsed)
{
	bool	finished;

	simulation_finished(philo->rules, &finished);
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK) \
		&& !finished)
		printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
	else if ((status == EAT) && !finished)
		printf("%-6ld %d is eating\n", elapsed, philo->id);
	else if ((status == SLEEP) && !finished)
		printf("%-6ld %d is sleeping\n", elapsed, philo->id);
	else if ((status == THINK) && !finished)
		printf("%-6ld %d is thinking\n", elapsed, philo->id);
	else if ((status == DIED) && !finished)
		printf("%-6ld %d died\n", elapsed, philo->id);
}

int	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	elapsed;
	bool	finished;

	elapsed = gettime(MILLISECONDS) - philo->rules->start_time;
	if (philo->full)
		return (0);
	if (get_bool(&philo->rules->rule_mutex, &philo->rules->end_simulation, \
	&finished))
		return (1);
	if (finished && status != DIED)
		return (0);
	if (handle_mutex(&philo->rules->write_lock, LOCK))
		return (1);
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
		print_status_line(status, philo, elapsed);
	if (handle_mutex(&philo->rules->write_lock, UNLOCK))
		return (1);
	return (0);
}

// int write_status(t_philo_status status, t_philo *philo, bool debug)
// {
// 	long	elapsed;
//     t_rules *rules = philo->rules;

// 	if (pthread_mutex_trylock(&rules->write_lock) != 0)
//     	printf("Couldn't acquire write_lock\n");
//     // pthread_mutex_lock(&rules->write_lock);       // 1) grab the single print lock
//     if (rules->end_simulation && status != DIED)  // 2) if someone already died, skip all but DIED
//     {
//         pthread_mutex_unlock(&rules->write_lock);
//         return 0;
//     }
// 	elapsed = gettime(MILLISECONDS) - philo->rules->start_time;
// 	if (debug)
// 		write_status_debug(status, philo, elapsed);
// 	else
// 		print_status_line(status, philo, elapsed);
// 	if (handle_mutex(&philo->rules->write_lock, UNLOCK))
// 		return (1);
//     return 0;
// }

// int write_status(t_philo_status status, t_philo *philo, bool debug)
// {
// 	long elapsed;
// 	t_rules *rules = philo->rules;
// 	int rc;

// 	rc = pthread_mutex_lock(&rules->write_lock);
// 	if (rc != 0)
// 	{
// 		printf("Error: Failed to lock write_lock (code %d)\n", rc);
// 		return 1;
// 	}

// 	if (rules->end_simulation && status != DIED)
// 	{
// 		rc = pthread_mutex_unlock(&rules->write_lock);
// 		if (rc != 0)
// 			printf("Error: Failed to unlock write_lock (code %d)\n", rc);
// 		return 0;
// 	}

// 	elapsed = gettime(MILLISECONDS) - rules->start_time;

// 	if (debug)
// 		write_status_debug(status, philo, elapsed);
// 	else
// 		print_status_line(status, philo, elapsed);

// 	rc = pthread_mutex_unlock(&rules->write_lock);
// 	if (rc != 0)
// 	{
// 		printf("Error: Failed to unlock write_lock (code %d)\n", rc);
// 		return 1;
// 	}

// 	return 0;
// }
