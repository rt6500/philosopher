/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:02:56 by rseki             #+#    #+#             */
/*   Updated: 2025/04/24 09:02:58 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h> //LLONG_MAX
#include <pthread.h>
#include <stdio.h>    //printf
#include <sys/time.h> //gettimeofday

typedef struct s_philo
{
	size_t			id;
	pthread_mutex_t	fork_r;
	pthread_mutex_t	fork_l;
	pthread_t		thread;
	long			last_meal;
	size_t			total_meals;
}					t_philo;

typedef struct s_rules
{
	size_t			num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	size_t			meals_required;
	struct timeval	start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	red_flag;
	pthread_mutex_t	print_lock;
	t_philo			*philos;
}					t_rules;

// main.c
int					main(int argc, char **argv);

// validate_input.c
int					is_number(const char *s);
int					is_valid_input(int argc, char **argv);