/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilis.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:27:16 by rseki             #+#    #+#             */
/*   Updated: 2025/04/29 13:27:17 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static inline bool	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

long	ft_atol(const char *str)
{
	long	num;

	num = 0;
	if (*str == '+')
		str++;
	while (is_digit(*str))
		num = (num * 10) + *str++ - '0';
	return (num);
}

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
