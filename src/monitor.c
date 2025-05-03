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
static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (false);
	elapsed = gettime(MILLISECONDS) - get_long(&philo->philo_mutex,
			&philo->last_meal_time);
	time_to_die = philo->rules->time_to_die / 1000L;
	// printf("DEBUG: Philo %d elapsed = %ld, time_to_die = %ld\n", philo->id,
	// 	elapsed, time_to_die);

	if (elapsed > time_to_die)
	{
		printf("Philo %d: elapsed = %ld, last_meal_time = %ld, now = %ld, threshold = %ld\n",
			philo->id, elapsed, get_long(&philo->philo_mutex, &philo->last_meal_time),
			gettime(MILLISECONDS), philo->rules->time_to_die);
		return (true);
	}
	return (false);
}

void	*monitor_dinner(void *data)
{
	int		i;
	t_rules	*rule;

	rule = (t_rules *)data;
	// make sure all philo runnning
	// spinlock till all thread run
	while (!all_threads_running(&rule->rule_mutex, &rule->threads_running_nbr,
			rule->num_philos))
			;
	// constantly check time to die
	while (!simulation_finished(rule))
	{
		i = -1;
		while (++i < rule->num_philos)
		{
			if (philo_died(rule->philos + i) && !simulation_finished(rule))
			{
				set_bool(&rule->rule_mutex, &rule->end_simulation, true);
				// printf("end_simulation: %d, philo_id: %d\n",
				// 	rule->end_simulation, rule->philos[i].id);
				write_status(DIED, rule->philos + i, DEBUG_MODE);
				printf(" Simulation finished = %d | Philo %d status: died = %d\n",
					get_bool(&rule->rule_mutex, &rule->end_simulation),
					rule->philos[i].id, philo_died(rule->philos + i));
			}
		}
	}
	return (NULL);
}
