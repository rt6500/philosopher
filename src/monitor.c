/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 07:53:46 by rseki             #+#    #+#             */
/*   Updated: 2025/05/02 08:06:26 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
last_meal --> last_meal
*/
static int	philo_died(t_philo *philo, bool *died)
{
	long	elapsed;
	long	time_to_die;
	bool	full;
	long	last_meal;

	if (get_bool(&philo->philo_mutex, &philo->full, &full))
		return (1);
	if (full)
	{
		*died = false;
		return (0);
	}
	elapsed = gettime(MILLISECONDS) - get_long(&philo->philo_mutex,
			&philo->last_meal_time);
	time_to_die = philo->rules->time_to_die / 1000L;
	*died = (elapsed > time_to_die);
	return (0);
}

int	*monitor_dinner(void *data)
{
	int		i;
	t_rules	*rule;
	bool	died;

	rule = (t_rules *)data;
	// make sure all philo runnning
	// spinlock till all thread run
	while (!all_threads_running(&rule->rule_mutex, &rule->threads_running_nbr,
			rule->num_philos))
			usleep(1);
	// constantly check time to die
	while (!simulation_finished(rule))
	{
		i = -1;
		while (++i < rule->num_philos)
		{
			if (philo_died(rule->philos + i, &died))
				return (1);
			else if (died && !simulation_finished(rule))
			{
				write_status(DIED, rule->philos + i, DEBUG_MODE);
				set_bool(&rule->rule_mutex, &rule->end_simulation, true);
				// printf(" Simulation finished = %d | Philo %d status: died = %d\n",
				// 	get_bool(&rule->rule_mutex, &rule->end_simulation, &died),
				// 	rule->philos[i].id, philo_died(rule->philos + i, &died));
			}
		}
	}
	return (0);
}
