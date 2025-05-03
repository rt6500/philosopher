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

static inline bool	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

static long	ft_atol(const char *str)
{
	long	num;

	num = 0;
	if (*str == '+')
		str++;
	while (is_digit(*str))
		num = (num * 10) + *str++ - '0';
	return (num);
}

/*					ms		ms		ms
* ./philo	5	100	100	100	[3]
	[0]		[1]	[2]	[3]	[4]	[5]
	1)actual numbrs
	2)not > INTMAX
	3)timestamp > 60ms
	USLEEP need time in microseconds
*/
void	parse_input(char **argv, t_rules *rule)
{
	rule->num_philos = ft_atol(argv[1]);
	rule->time_to_die = ft_atol(argv[2]) * 1e3;
	rule->time_to_eat = ft_atol(argv[3]) * 1e3;
	rule->time_to_sleep = ft_atol(argv[4]) * 1e3;
	if (argv[5])
		rule->limit_meals = ft_atol(argv[5]);
	else
		rule->limit_meals = -1;
}

static void	assign_forks(t_philo *philo, t_fork *forks, int philo_position)
{
	philo->first_fork = &forks[(philo_position + 1) % philo->rules->num_philos];
	philo->second_fork = &forks[philo_position];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[philo_position];
		philo->second_fork = &forks[(philo_position + 1)
			% philo->rules->num_philos];
	}
}

static void	philo_init(t_rules *rule)
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
		handle_mutex(&rule->philos->philo_mutex, INIT);
		assign_forks(philo, rule->forks, i);
	}
}

int	init_data(char **argv, t_rules *rule)
{
	int i;

	i = 0;
	parse_input(argv, rule);
	rule->end_simulation = false;
	rule->all_threads_ready = false;
	rule->threads_running_nbr = 0;
	rule->philos = malloc(rule->num_philos * sizeof(t_philo));
	if (!rule->philos)
		return (1);
	rule->forks = malloc(rule->num_philos * sizeof(t_fork));
	if (!rule->forks)
		return (1);
	handle_mutex(&rule->rule_mutex, INIT);
	handle_mutex(&rule->write_lock, INIT);
	while (i < rule->num_philos)
	{
		handle_mutex(&rule->forks[i].fork, INIT);
		rule->forks[i].fork_id = i;
		i++;
	}
	philo_init(rule);
	// pthread_mutex_init(&rule->print_lock, NULL);
	// gettimeofday(&rule->start_time, NULL);
	print_assigned_forks(rule);
	return (0);
}