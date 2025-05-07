/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 07:39:17 by rseki             #+#    #+#             */
/*   Updated: 2025/04/29 10:02:46 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	dinner_setup(t_philo *philo)
{
	bool	finished;

	if (wait_all_threads(philo->rules))
		return (1);
	if (increase_long(&philo->rules->rule_mutex, \
		&philo->rules->threads_running_nbr))
		return (1);
	if (set_long(&philo->philo_mutex, &philo->last_meal_time, \
		gettime(MILLISECONDS)))
		return (1);
	if (de_synchronize_philo(philo))
		return (1);
	if (simulation_finished(philo->rules, &finished))
		return (1);
	return (0);
}

/*
0) wait all philos, synchro start
1) endless loop philo
// data =  &rule->philos[i]
*/

void	*dinner_simulation(void *data)
{
	t_philo	*philo;
	bool	finished;

	philo = (t_philo *)data;
	if (dinner_setup(philo))
		return (NULL);
	while (!simulation_finished(philo->rules, &finished) && !finished)
	{
		if (philo->full)
			break ;
		if (eat(philo))
			return (NULL);
		if (write_status(SLEEP, philo, DEBUG_MODE))
			return (NULL);
		if (smart_sleep(philo->rules->time_to_sleep, philo->rules))
			return (NULL);
		if (think(philo, false))
			return (NULL);
	}
	return (philo);
}

/*
actural dinner
./philo 5 100 100 100 [4]
0) if no meals, return (->[0]);
0.1) If only one philo->ad hoc function
1)Create all threads, all philosophers.
2)Create a monitor thread for death of philos.
3)Synchronize the beggining of the simulation
	pthread_create->philo starts running.
	every philo starts simultaneously.
4) JOIN everyone.
*/

int	start_dinner(t_rules *rule)
{
	if (rule->limit_meals == 0)
		return (1);
	if (create_threads(rule))
		return (1);
	rule->start_time = gettime(MILLISECONDS);
	if (set_bool(&rule->rule_mutex, &rule->all_threads_ready, true))
		return (1);
	if (DEBUG_MODE == 1)
		print_assigned_forks(rule);
	if (join_threads(rule))
		return (1);
	return (0);
}
