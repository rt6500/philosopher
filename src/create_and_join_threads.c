/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_and_join_threads.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:56:39 by rseki             #+#    #+#             */
/*   Updated: 2025/05/06 15:56:40 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	create_threads(t_rules *rule)
{
	int	i;
	int	status;

	if (rule->num_philos == 1)
	{
		status = pthread_create(&rule->philos[0].thread_id, NULL, \
			one_philo, &rule->philos[0]);
		if (handle_thread_error(status, CREATE))
			return (1);
		rule->monitor = 0;
		return (0);
	}
	i = -1;
	while (++i < rule->num_philos)
	{
		status = pthread_create(&rule->philos[i].thread_id, NULL, \
			dinner_simulation, &rule->philos[i]);
		if (handle_thread_error(status, CREATE))
			return (1);
	}
	status = pthread_create(&rule->monitor, NULL, monitor_dinner, \
	rule);
	return (handle_thread_error(status, CREATE));
}

static int	join_philo_threads(t_rules *rule)
{
	int		i;
	int		status;
	void	*ret;

	i = -1;
	while (++i < rule->num_philos)
	{
		ret = NULL;
		if (rule->philos[i].thread_id)
			status = pthread_join(rule->philos[i].thread_id, &ret);
		if (handle_thread_error(status, JOIN))
			return (1);
		if (ret == NULL)
			return (printf("Error\nPhilosopher thread returned NULL\n"), 1);
	}
	return (0);
}

int	join_threads(t_rules *rule)
{
	int		status;
	void	*ret;

	status = 0;
	if (join_philo_threads(rule))
		return (1);
	if (set_bool(&rule->rule_mutex, &rule->end_simulation, true))
		return (1);
	ret = NULL;
	if (rule->monitor)
		status = pthread_join(rule->monitor, &ret);
	if (handle_thread_error(status, JOIN))
		return (1);
	if (ret != 0)
		return (printf("Error\nMonitor thread error\n"), 1);
	return (0);
}
