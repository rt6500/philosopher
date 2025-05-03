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
void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);
}

bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	bool	ret;

	handle_mutex(mutex, LOCK);
	// READING
	ret = *value;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}

// LONG
void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	handle_mutex(mutex, LOCK);
	ret = *value;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}

// simulation finished?
// bool	simulation_finished(t_rules *rule)
// {
// 	return (get_bool(&rule->rule_mutex, &rule->end_simulation));
// }

bool	simulation_finished(t_rules *rule)
{
	bool	result;

	result = get_bool(&rule->rule_mutex, &rule->end_simulation);
	// printf("DEBUG: simulation_finished = %d\n", result);
	return (result);
}
