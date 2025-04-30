/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilis_dinner.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:04:23 by rseki             #+#    #+#             */
/*   Updated: 2025/04/30 09:40:28 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	smart_sleep(long duration, t_rules *rules)
{
	long	start;

	start = gettime(MICROSECONDS);
	while (!simulation_finished(rules))
	{
		if (gettime(MICROSECONDS) - start >= duration)
			break ;
		usleep(500);
	}
}

void	log_action(t_philo *philo, const char *action)
{
	long	timestamp;

	timestamp = gettime(MICROSECONDS) - philo->rules->start_time;
	if (!simulation_finished(philo->rules))
		printf("%ld %d %s\n", timestamp / 1000, philo->id, action);
}

int	eat(t_philo *philo)
{
	handle_mutex(philo->first_fork, LOCK);
	log_action(philo, "has taken a fork");
	handle_mutex(philo->second_fork, LOCK);
	log_action(philo, "has taken a fork");
	set_long(&philo->rules->rule_mutex, &philo->last_meal_time,
		gettime(MICROSECONDS));
}

int	sleep(t_philo *philo)
{
	log_action(philo, "is sleeping");
	smart_sleep(philo->rules->time_to_sleep, philo->rules);
}

int	think(t_philo philo)
{
}
