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

	if (!is_valid_input(argc, argv))
		return (printf("Error\nInput is not valid\n"), 1);
	else
	{
		if (init_data(argv, &rule))
			return (printf("Error\nInitialization failed.\n"), 1);
		if (start_dinner(&rule))
			return (printf("Error\nSimulation failed\n"), 1);
		clean(&rule);
	}
	return (0);
}
