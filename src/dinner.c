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

void	think(t_philo *philo, bool pre_simulation)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_simulation)
		write_status(THINK, philo, DEBUG_MODE);
	if (philo->rules->num_philos % 2 == 0)
		return ;
	t_eat = philo->rules->time_to_eat;
	t_sleep = philo->rules->time_to_sleep;
	t_think = t_eat * 2- t_sleep; // available time to think
	if (t_think < 0)
		t_think = 0;
	smart_sleep(t_think * 0.42, philo->rules);
}

/*
for only one philo
1)fake lock to the fork.
2)sleep untill the monitor will bust it
*/
void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->rules);
	set_long(&philo->philo_mutex, &philo->last_meal_time, \
		gettime(MILLISECONDS));
	increase_long(&philo->rules->rule_mutex, \
		&philo->rules->threads_running_nbr);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!simulation_finished(philo->rules))
		usleep(100);
	return (NULL);
}

/*
thinking routine
*/
// static void	think(t_philo *philo)
// {
// 	write_status(THINK, philo, DEBUG_MODE);
// }

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
			gettime(MILLISECONDS));
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

	philo = (t_philo *)data;
	// spinlock
	wait_all_threads(philo->rules);
	// sync with monitor
	// increase a rule variable counter, with all threads running
	increase_long(&philo->rules->rule_mutex,
		&philo->rules->threads_running_nbr);
	// set last meal time
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		gettime(MILLISECONDS));
	// desync philos
	de_synchronize_philo(philo);
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
		think(philo, false);
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
		handle_thread(&rule->philos[0].thread_id, one_philo, &rule->philos[0],
			CREATE);
	else
	{
		while (++i < rule->num_philos)
			handle_thread(&rule->philos[i].thread_id, dinner_simulation,
				&rule->philos[i], CREATE);
	}
	// start of simutation
	rule->start_time = gettime(MILLISECONDS);
	// moniter
	handle_thread(&rule->monitor, monitor_dinner, rule, CREATE);
	// now all threads are ready.
	set_bool(&rule->rule_mutex, &rule->all_threads_ready, true);
	// wait for everyone
	i = -1;
	while (++i < rule->num_philos)
		handle_thread(&rule->philos[i].thread_id, NULL, NULL, JOIN);
	// if we manage to reach this linke, all phlilos are full!
	 set_bool(&rule->rule_mutex, &rule->end_simulation, true);
	 handle_thread(&rule->monitor, NULL, NULL, JOIN);
}
