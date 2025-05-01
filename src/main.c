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

#define MAX_TIME 1000       // Total simulation time in ms
#define TIME_STEP 50        // Interval between log entries in ms
#define MAX_PHILOS 5
#define STATES 3

enum e_state { THINKING = 0, EATING = 1, SLEEPING = 2 };

// This array simulates philosopher state logs for the example
int philo_log[MAX_PHILOS][MAX_TIME / TIME_STEP];

const char *state_symbols[] = {
	"\033[0;36m░\033[0m", // THINKING: cyan
	"\033[0;32m█\033[0m", // EATING: green
	"\033[0;34m▓\033[0m"  // SLEEPING: blue
};

void print_timeline(int num_philos, int duration_ms)
{
	int steps = duration_ms / TIME_STEP;

	for (int i = 0; i < num_philos; i++)
	{
		printf("Philo %d: ", i + 1);
		for (int t = 0; t < steps; t++)
		{
			int state = philo_log[i][t];
			printf("%s", state_symbols[state]);
		}
		printf("\n");
	}
}

void mock_log_data()
{
	// For demo: alternate states
	for (int t = 0; t < MAX_TIME / TIME_STEP; t++)
	{
		for (int p = 0; p < MAX_PHILOS; p++)
		{
			if (t % 3 == 0)
				philo_log[p][t] = THINKING;
			else if (t % 3 == 1)
				philo_log[p][t] = EATING;
			else
				philo_log[p][t] = SLEEPING;
		}
	}
}

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
		// usleep(50000);
		// how_long(rule);
		// 3) dinner
		start_dinner(&rule);
		// clean(rule);
	}
	mock_log_data();
	print_timeline(MAX_PHILOS, MAX_TIME);
	return (0);
}
