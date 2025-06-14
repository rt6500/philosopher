/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilis_timekeeper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:28:22 by rseki             #+#    #+#             */
/*   Updated: 2025/04/29 13:28:23 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
time_code is set as seconds, milliseconds, and microseconds.
*/
long	gettime(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (printf("Gettimeofday failed\n"), -1);
	if (time_code == SECONDS)
		return (tv.tv_sec + tv.tv_usec / 1000000L);
	else if (time_code == MILLISECONDS)
		return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
	else if (time_code == MICROSECONDS)
		return (tv.tv_sec * 1000000L + tv.tv_usec);
	else
		return (printf("Wrong input to gettime"), -1);
}
