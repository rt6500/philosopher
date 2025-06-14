/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:21:06 by rseki             #+#    #+#             */
/*   Updated: 2025/04/29 09:21:08 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
Module containing setters_getters
*/

// BOOL
int	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	if (handle_mutex(mutex, LOCK))
		return (1);
	*dest = value;
	if (handle_mutex(mutex, UNLOCK))
		return (1);
	return (0);
}

int	get_bool(pthread_mutex_t *mutex, bool *value, bool *out)
{
	if (handle_mutex(mutex, LOCK))
		return (1);
	*out = *value;
	if (handle_mutex(mutex, UNLOCK))
		return (1);
	return (0);
}

// LONG
int	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	if (handle_mutex(mutex, LOCK))
		return (1);
	*dest = value;
	if (handle_mutex(mutex, UNLOCK))
		return (1);
	return (0);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	if (handle_mutex(mutex, LOCK))
		return (-1);
	ret = *value;
	if (handle_mutex(mutex, UNLOCK))
		return (-1);
	return (ret);
}

// simulation finished?
int	simulation_finished(t_rules *rule, bool *out)
{
	return (get_bool(&rule->rule_mutex, &rule->end_simulation, out));
}
