/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:52:46 by rseki             #+#    #+#             */
/*   Updated: 2025/05/06 15:52:47 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
for debugging
*/

void	print_assigned_forks(t_rules *rules)
{
	int		i;
	t_philo	*philo;

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

// void	how_long(t_rules rule)
// {
// 	struct timeval	now;
// 	long			elapsed_ms;

// 	gettimeofday(&now, NULL);
// 	elapsed_ms = (now.tv_sec - rule.start_time.tv_sec) * 1e3 + (now.tv_usec
// 			- rule.start_time.tv_usec) / 1e3;
// 	printf("elapsed time: %ld ms\n", elapsed_ms);
// }
