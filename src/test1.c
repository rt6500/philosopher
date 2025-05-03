#include "../philo.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assume your original code is included or properly forward-declared here.
// Also assume `t_fork` and `handle_mutex()` are defined elsewhere.
// For test purposes, we can mock `t_fork` and `handle_mutex`.
// Your original structs and functions go here...

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
		assign_forks(philo, rule->forks, i);
	}
}

int	init_data(char **argv, t_rules *rule)
{
	int	i;

	i = 0;
	parse_input(argv, rule);
	rule->end_simulation = false;
	rule->all_threads_ready = false;
	rule->philos = malloc(rule->num_philos * sizeof(t_philo));
	if (!rule->philos)
		return (1);
	rule->forks = malloc(rule->num_philos * sizeof(t_fork));
	if (!rule->forks)
		return (1);
	handle_mutex(&rule->rule_mutex, INIT);
	while (i < rule->num_philos)
	{
		handle_mutex(&rule->forks[i].fork, INIT);
		rule->forks[i].fork_id = i;
		i++;
	}
	philo_init(rule);
	return (0);
}

// Add mode constants if needed:
#define INIT 0
#define DESTROY 1

int	main(int argc, char **argv)
{
	t_rules	rule;
	int		i;
	char	*fake_argv[] = {"philo", "4", "800", "200", "200", NULL};
	t_philo	*p;

	// Fake argv for 4 philosophers: 4 800 200 200
	if (init_data(fake_argv, &rule) != 0)
	{
		printf("Initialization failed.\n");
		return (1);
	}
	printf("=== Rule Variables ===\n");
	printf("num_philos: %ld\n", rule.num_philos);
	printf("time_to_die: %ld\n", rule.time_to_die);
	printf("time_to_eat: %ld\n", rule.time_to_eat);
	printf("time_to_sleep: %ld\n", rule.time_to_sleep);
	printf("limit_meals: %ld\n", rule.limit_meals);
	printf("end_simulation: %d\n", rule.end_simulation);
	printf("all_threads_ready: %d\n", rule.all_threads_ready);
	printf("\n=== Philosopher Variables ===\n");
	for (i = 0; i < rule.num_philos; i++)
	{
		p = &rule.philos[i];
		printf("Philo %d:\n", p->id);
		printf("  Full: %d\n", p->full);
		printf("  Total meals: %ld\n", p->total_meals);
		printf("  Fork IDs: %d (first), %d (second)\n", p->first_fork->fork_id,
			p->second_fork->fork_id);
		printf("  Rule pointer matches: %s\n",
			p->rules == &rule ? "yes" : "no");
	}
	// Cleanup (not required for test, but good practice)
	for (i = 0; i < rule.num_philos; i++)
		handle_mutex(&rule.forks[i].fork, DESTROY);
	handle_mutex(&rule.rule_mutex, DESTROY);
	free(rule.forks);
	free(rule.philos);
	return (0);
}
