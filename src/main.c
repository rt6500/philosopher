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

int	main(int argc, char **argv)
{
	t_rules	rule;

	// 1) errors checking
	if (!is_valid_input(argc, argv))
		return (printf("Error\n"), 1);
	else
	{
		// 2) filling data to rule
		init_data(argv, &rule);
		printf("num: %ld, die: %ld, eat: %ld, sleep: %ld, limit: %ld\n",
			rule.num_philos, rule.time_to_die, rule.time_to_eat,
			rule.time_to_sleep, rule.limit_meals);
		usleep(50000);
		how_long(rule);
		// 3) dinner
		// start_dinner(rule);
		// clean(rule);
	}
	return (0);
}
