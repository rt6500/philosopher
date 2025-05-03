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
/*

*/
void	smart_sleep(long duration, t_rules *rules)
{
	long	start;
	long	elapsed;
	long	remain;

	start = gettime(MICROSECONDS);
	while (gettime(MICROSECONDS) - start < duration)
	{
		//1) is simulation finished?
		if (simulation_finished(rules))
			break ;
		elapsed = gettime(MICROSECONDS) - start;
		remain = duration - elapsed;
		if (remain > 1e3)
			usleep(remain / 2);
		else
		{
			while (gettime(MICROSECONDS) - start < duration)
			;
		}
	}
}

/*
for debugging
*/

void	print_assigned_forks(t_rules *rules)
{
	int			i;
	t_philo		*philo;

	i = 0;
	while (i < rules->num_philos)
	{
		philo = &rules->philos[i];
		printf("Philo %d:\n", philo->id);
		printf("  First fork : %d\n", philo->first_fork->fork_id);
		printf("  Second fork: %d\n", philo->second_fork->fork_id);
		i++;
	}
}

