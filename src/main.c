/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:02:24 by rseki             #+#    #+#             */
/*   Updated: 2025/04/24 09:02:25 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi_p(char *s)
{
	long	i;

	i = 0;
	if (*s == '+')
		s++;
	while (*s)
	{
		i = i * 10 + *s - '0';
		s++;
	}
	return (i);
}

void	parse_input(int argc, char **argv, t_rules *rule)
{
	rule->time_to_die = 0;
	rule->time_to_eat = 0;
	rule->time_to_sleep = 0;
	rule->meals_required = 0;
	rule->num_philos = ft_atoi_p(argv[1]);
	rule->time_to_die = ft_atoi_p(argv[2]);
	rule->time_to_eat = ft_atoi_p(argv[3]);
	rule->time_to_sleep = ft_atoi_p(argv[4]);
	if (argc == 5)
		rule->meals_required = ft_atoi_p(argv[5]);
}

void	init(int argc, char **argv, t_rules *rule)
{
	gettimeofday(&rule->start_time, NULL);
	// printf("current time: %i\n", gettimeofday);
	parse_input(argc, argv, rule);
}

int	main(int argc, char **argv)
{
	struct timeval	time;

	argc = 0;
	argv = NULL;
	gettimeofday(&time, NULL);
	printf("time: %ld\n", time.tv_sec * 1000);
	return (0);
}

// int	main(int argc, char **argv)
// {
// 	t_rules	*rule;

// 	rule = NULL;
// 	if (!is_valid_input(argc, argv))
// 		return (printf("Error\n"), 1);
// 	else
// 	{
// 		init(argc, argv, rule);
// 	}
// 	return (0);
// }
