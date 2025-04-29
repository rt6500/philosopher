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

*/

/*
0) wait all philos, synchro start
1) endless loop philo
*/

void	*dinner_simulation(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->rules);
	return (philo);
}

/*
actural dinner
./philo 5 100 100 100 [4]
0) if no meals, return ->[0];
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
		;//TODO
	else
	{
		while (++i < rule->num_philos)
			handle_thread(&rule->philos[i].thread_id, dinner_simulation, \
                &rule->philos[i], CREATE);
	}
    // start of simutation
    rule->start_time = gettime(MICROSECONDS);

    //now all threads are ready.
	set_bool(&rule->rule_mutex, &rule->all_threads_ready, true);

    //wait for everyone
    i = -1;
    while (++i < rule->num_philos)
        handle_thread(&rule->philos[i].thread_id, NULL, NULL, JOIN);
    printf("full:%d %d %d %d\n", rule->philos->full, rule->philos->full, rule->philos->full, rule->philos->full);
}
