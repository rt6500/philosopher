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

void	parse_input(char **argv, t_rules *rule)
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

int	init_data(char **argv, t_rules *rule)
{
	int	i;

	i = 0;
	parse_input(argv, rule);
	rule->end_simulation = false;
	rule->philos = malloc(rule->num_philos * sizeof(t_philo));
	if (!rule->philos)
		return (1);
	rule->forks = malloc(rule->num_philos * sizeof(t_fork));
	if (!rule->forks)
		return (1);
	while (i < rule->num_philos)
	{
		handle_mutex(&rule->forks[i].fork, INIT);
		rule->forks[i].fork_id = i;
		i++;
	}
	pthread_mutex_init(&rule->print_lock, NULL);
	gettimeofday(&rule->start_time, NULL);
	return (0);
}