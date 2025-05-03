/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilis_free_malloc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:27:16 by rseki             #+#    #+#             */
/*   Updated: 2025/04/29 13:27:17 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// void	*malloc_with_check(size_t size)
// {
// 	void	*p;

// 	p = malloc(size);
// 	if (p == NULL)
// 		return (write(1, "Error\n", 6), NULL);
// 	else
// 		return (p);
// }

void	clean(t_rules *rule)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < rule->num_philos)
	{
		philo = rule->philos + i;
		handle_mutex(&philo->philo_mutex, DESTROY);
	}
	handle_mutex(&rule->rule_mutex, DESTROY);
	handle_mutex(&rule->write_lock, DESTROY);
	free(rule->forks);
	free(rule->philos);
}
