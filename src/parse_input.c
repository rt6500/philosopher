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
	rule->time_to_die = ft_atol(argv[2]) / 1e3;
	rule->time_to_eat = ft_atol(argv[3]) / 1e3;
	rule->time_to_sleep = ft_atol(argv[4]) / 1e3;
	if (argv[5])
		rule->limit_meals = ft_atol(argv[5]);
	else
		rule->limit_meals = -1;
}

int	init_data(char **argv, t_rules *rule)
{
	parse_input(argv, rule);
	rule->end_simulation = false;
	if (malloc_with_check(rule->num_philos * sizeof(t_philo)))
		return (1);
	else if (malloc_with_check(rule->num_philos * sizeof(t_philo)))
		return (1);
	else
		gettimeofday(&rule->start_time, NULL);
}