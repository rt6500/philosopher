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
	if (wait_for_start_signal(philo->rules))
		return (1);
	if (increase_long(&philo->rules->rule_mutex,
			&philo->rules->threads_running_nbr))
		return (1);
	if (set_long(&philo->philo_mutex, &philo->last_meal_time,
			gettime(MILLISECONDS)))
		return (1);
	return (0);
}

static int	philo_routine_loop(t_philo *philo)
{
	bool	finished;
	int		rc;

	while (!simulation_finished(philo->rules, &finished) && !finished)
	{
		if (philo->full)
			break ;
		rc = eat(philo);
		if (rc == 1)
			return (1);
		else if (rc == 2)
			break ;
		if (simulation_finished(philo->rules, &finished))
			return (1);
		if (finished)
			break ;
		if (write_status(SLEEP, philo, DEBUG_MODE))
			return (1);
		if (smart_sleep(philo->rules->time_to_sleep * 1e3, philo->rules))
			return (1);
		if (think(philo, false))
			return (1);
	}
	return (0);
}

/*
0) wait all philos, sync start
1) endless loop philo
// data =  &rule->philos[i]
*/
void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (dinner_setup(philo))
		return (NULL);
	if (philo_routine_loop(philo))
		return (NULL);
	return (philo);
}

/*
actural dinner
./philo 5 100 100 100 [4]
0)if no meals, return (->[0]);
1)Create all threads, all philosophers & a monitor thread. Start simulation.
2)Setting the start time.
3)JOIN everyone.
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
	if (join_threads(rule))
		return (1);
	return (0);
}
