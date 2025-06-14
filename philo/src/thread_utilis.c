/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utilis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:04:23 by rseki             #+#    #+#             */
/*   Updated: 2025/04/30 09:40:28 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"
/*
usleep if there is planty of time till the duration.
otherwise check duration often, and when reach the duration, return 0
in microseconds!
*/
int	smart_sleep(long duration, t_rules *rules)
{
	long	start;
	long	elapsed;
	long	remain;
	bool	finished;

	start = gettime(MICROSECONDS);
	while (gettime(MICROSECONDS) - start < duration)
	{
		if (simulation_finished(rules, &finished))
			return (1);
		if (finished)
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
	return (0);
}
