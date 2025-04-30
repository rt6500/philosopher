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

#include <limits.h>  //LLONG_MAX
#include <pthread.h> // mutex: init destroy lock unlock,
						// threads : create join detach
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>    //printf
#include <stdlib.h>   //malloc free
#include <sys/time.h> //gettimeofday
#include <unistd.h>   //write, usleep

//*** structures ***/
typedef struct s_rules			t_rules;

/*OPCOD for mutexs | thread functions*/
typedef enum	e_opcode
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_opcode;

/*CODES for time*/
typedef enum	e_time_code
{
	SECONDS,
	MILLISECONDS,
	MICROSECONDS,
}	t_time_code;

/*struct for forks*/
typedef struct s_fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}					t_fork;

/*struct for philosophers*/
typedef struct s_philo
{
	int				id;
	t_fork	*first_fork;
	t_fork	*second_fork;
	pthread_t		thread_id;
	long			last_meal_time;
	long			total_meals;
	bool			full;
	t_rules			*rules;
}					t_philo;

/*struct for saving inputs, time, counters, flags, and structs*/
typedef struct s_rules
{
	long			num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			limit_meals;
	long			start_time;
	bool			end_simulation;
	bool			all_threads_ready;
	pthread_mutex_t	rule_mutex; // avoid races while reading from rules.
	t_fork			*forks;
	t_philo			*philos;
}					t_rules;

// main.c
int					main(int argc, char **argv);

// validate_input.c
int					is_number(const char *s);
int					is_valid_input(int argc, char **argv);
void				parse_input(char **argv, t_rules *rule);

// utilis_free_malloc.c
void				free_array_philo(t_philo **array);
void				free_array_fork(t_fork **array);

// init.c
void				parse_input(char **argv, t_rules *rule);
int					init_data(char **argv, t_rules *rule);

// utilis_thread_mutex.c
int					handle_mutex(pthread_mutex_t *mutex, t_opcode opcode);
int					handle_thread(pthread_t *thread, \
	void *(*foo)(void *), void *data, t_opcode opcode);

//getters_setters.c
void    set_bool(pthread_mutex_t *mutex, bool *dest, bool value);
bool    get_bool(pthread_mutex_t *mutex, bool *value);
void    set_long(pthread_mutex_t *mutex, long *dest, long value);
long    get_long(pthread_mutex_t *mutex, long *value);
bool    simulation_finished(t_rules *rule);

//dinner.c
void	*dinner_simulation(void *data);
void	start_dinner(t_rules *rule);

//sync_utilis.c
void    wait_all_threads(t_rules *rule);

//utilis_timekeeper.c
void				error_exit(const char *error);
// void				how_long(t_rules rule);
long    			gettime(t_time_code time_code);