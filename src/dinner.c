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

/*
thinking routine
*/
static void	think(t_philo *philo)
{
	write_status(THINK, philo, DEBUG_MODE);
}

/*
eat routine
1) grab the forks: 1st and 2nd.abort
2) update write eat, last_meal_time, total_meals,
3) release the forks
*/
static void	eat_spagetti(t_philo *philo)
{
	if (philo->full)
		return ;
	else
	{
		// 1)
		handle_mutex(&philo->first_fork->fork, LOCK);
		write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
		handle_mutex(&philo->second_fork->fork, LOCK);
		write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
		// 2)
		set_long(&philo->philo_mutex, &philo->last_meal_time,
			gettime(MICROSECONDS));
		philo->total_meals++;
		write_status(EAT, philo, DEBUG_MODE);
		smart_sleep(philo->rules->time_to_eat, philo->rules);
		if (philo->rules->limit_meals > 0
			&& philo->total_meals == philo->rules->limit_meals)
			set_bool(&philo->philo_mutex, &philo->full, true);
		// 3)
		handle_mutex(&philo->first_fork->fork, UNLOCK);
		handle_mutex(&philo->second_fork->fork, UNLOCK);
	}
}
/*
0) wait all philos, synchro start
1) endless loop philo
// data =  &rule->philos[i]
*/

void	*dinner_simulation(void *data)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)data;
	i = -1;
	wait_all_threads(philo->rules);
	// set last meal time
	while (!simulation_finished(philo->rules))
	{
		// 1) am i full?
		if (philo->full) // thread safe? acccess by moniter also?
			break ;
		// 2) eat
		eat_spagetti(philo);
		// 3) sleep ->write status & smart usleep
		write_status(SLEEP, philo, DEBUG_MODE);
		smart_sleep(philo->rules->time_to_sleep, philo->rules);
		// 4) think
		think(philo);
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

void	start_dinner(t_rules *rule)
{
	int	i;

	i = -1;
	if (rule->limit_meals == 0)
		return ;
	else if (rule->num_philos == 1)
		; // TODO
	else
	{
		while (++i < rule->num_philos)
			handle_thread(&rule->philos[i].thread_id, dinner_simulation,
				&rule->philos[i], CREATE);
	}
	// start of simutation
	rule->start_time = gettime(MICROSECONDS);
	// now all threads are ready.
	set_bool(&rule->rule_mutex, &rule->all_threads_ready, true);
	// wait for everyone
	i = -1;
	while (++i < rule->num_philos)
		handle_thread(&rule->philos[i].thread_id, NULL, NULL, JOIN);
}
