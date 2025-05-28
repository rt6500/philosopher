/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:12:25 by rseki             #+#    #+#             */
/*   Updated: 2025/04/25 10:12:27 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*					ms		ms		ms
* ./philo	5	100	100	100	[3]
	[0]		[1]	[2]	[3]	[4]	[5]
	1)actual numbrs
	2)not > INTMAX
	USLEEP need time in microseconds
	time_to_die & time_to_eat & time_to_sleep are in milliseconds.
*/
static void	parse_input(char **argv, t_rules *rule)
{
	rule->num_philos = ft_atol(argv[1]);
	rule->time_to_die = ft_atol(argv[2]);
	rule->time_to_eat = ft_atol(argv[3]);
	rule->time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		rule->limit_meals = ft_atol(argv[5]);
	else
		rule->limit_meals = -1;
}

/*
fork_id:      0, 1, 2, 3, 4, ..., n-1, (n % n = 0)
philo_position: 0, 1, 2, 3, 4, ..., n-1
philo_id:       1, 2, 3, 4, 5, ..., n
1. Default assignment
2. Swqp if philosopher has even ID
*/
static void	assign_forks(t_philo *philo, t_fork *forks, int philo_position)
{
	philo->first_fork = &forks[(philo_position + 1) % philo->rules->num_philos];
	philo->second_fork = &forks[philo_position];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[philo_position];
		philo->second_fork = &forks[(philo_position + 1) \
			% philo->rules->num_philos];
	}
}

/*
philo->id: 1, 2, ...

*/
static int	philo_init(t_rules *rule)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < rule->num_philos)
	{
		philo = rule->philos + i;
		philo->id = i + 1;
		philo->full = false;
		philo->total_meals = 0;
		philo->rules = rule;
		if (handle_mutex(&philo->philo_mutex, INIT))
			return (1);
		assign_forks(philo, rule->forks, i);
	}
	return (0);
}

static int	alloc_resources(t_rules *rule)
{
	rule->philos = malloc(rule->num_philos * sizeof(t_philo));
	if (!rule->philos)
		return (1);
	rule->forks = malloc(rule->num_philos * sizeof(t_fork));
	if (!rule->forks)
		return (1);
	return (0);
}

/*
fork_id: 0, 1, 2, ...
*/
int	init_data(char **argv, t_rules *rule)
{
	int	i;

	i = -1;
	parse_input(argv, rule);
	rule->end_simulation = false;
	rule->all_threads_ready = false;
	rule->threads_running_nbr = 0;
	if (alloc_resources(rule))
		return (1);
	if (handle_mutex(&rule->rule_mutex, INIT))
		return (1);
	if (handle_mutex(&rule->write_lock, INIT))
		return (1);
	while (++i < rule->num_philos)
	{
		if (handle_mutex(&rule->forks[i].fork, INIT))
			return (1);
		rule->forks[i].fork_id = i;
	}
	if (philo_init(rule))
		return (1);
	return (0);
}
