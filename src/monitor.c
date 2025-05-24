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

static int	philo_died(t_philo *philo, bool *died)
{
	long	elapsed;
	long	time_to_die;
	bool	full;
	long	val;

	if (get_bool(&philo->philo_mutex, &philo->full, &full))
		return (1);
	if (full)
	{
		*died = false;
		return (0);
	}
	val = get_long(&philo->philo_mutex, &philo->last_meal_time);
	if (val < 0)
		return (1);
	elapsed = gettime(MILLISECONDS) - val;
	time_to_die = philo->rules->time_to_die / 1000L;
	*died = (elapsed > time_to_die);
	return (0);
}

static int	check_philos_state(t_rules *rule)
{
	int		i;
	bool	died;
	bool	sim_done;

	i = -1;
	while (++i < rule->num_philos)
	{
		if (philo_died(rule->philos + i, &died))
			return (1);
		if (died)
		{
			if (simulation_finished(rule, &sim_done))
				return (1);
			if (!sim_done)
			{
				if (write_status(DIED, rule->philos + i, DEBUG_MODE))
					return (1);
				if (set_bool(&rule->rule_mutex, &rule->end_simulation, \
					true))
					return (1);
			}
		}
	}
	return (0);
}

static int	wait_for_threads(t_rules *rule)
{
	bool	state;

	while (1)
	{
		if (all_threads_running(&rule->rule_mutex, &rule->threads_running_nbr, \
		rule->num_philos, &state))
			return (1);
		if (state)
			break ;
		usleep(1);
	}
	return (0);
}

void	*monitor_dinner(void *data)
{
	t_rules	*rule;
	bool	finished;

	rule = (t_rules *)data;
	if (wait_for_threads(rule))
		return ((void *)1);
	if (simulation_finished(rule, &finished))
		return ((void *)1);
	while (!finished)
	{
		if (check_philos_state(rule))
			return ((void *)1);
		if (simulation_finished(rule, &finished))
			return ((void *)1);
	}
	return (0);
}
