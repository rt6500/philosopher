/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:02:56 by rseki             #+#    #+#             */
/*   Updated: 2025/05/04 07:58:40 by rseki            ###   ########.fr       */
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

/*write function macro*/
#define DEBUG_MODE 1

//*** structures ***/
typedef struct s_rules	t_rules;

/*PHILO STATE*/
typedef enum e_status
{
	EAT,
	SLEEP,
	THINK,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}						t_philo_status;

/*OPCOD for mutexs | thread functions*/
typedef enum e_opcode
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}						t_opcode;

/*CODES for time*/
typedef enum e_time_code
{
	SECONDS,
	MILLISECONDS,
	MICROSECONDS,
}						t_time_code;

/*struct for forks*/
typedef struct s_fork
{
	pthread_mutex_t		fork;
	int					fork_id;
}						t_fork;

/*struct for philosophers*/
typedef struct s_philo
{
	int					id;
	t_fork				*first_fork;
	t_fork				*second_fork;
	pthread_t			thread_id;
	long				last_meal_time;
	long				total_meals;
	bool				full;
	pthread_mutex_t		philo_mutex;
	t_rules				*rules;
}						t_philo;

/*struct for saving inputs, time, counters, flags, and structs*/
typedef struct s_rules
{
	long				num_philos;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				limit_meals;
	long				start_time;
	bool				end_simulation;
	bool				all_threads_ready;
	long				threads_running_nbr;
	pthread_t			monitor;
	pthread_mutex_t		rule_mutex;
	pthread_mutex_t		write_lock;
	t_fork				*forks;
	t_philo				*philos;
}						t_rules;

// main.c
int						main(int argc, char **argv);

// validate_input.c
int						is_valid_input(int argc, char **argv);

// init.c
int						init_data(char **argv, t_rules *rule);

// utilis.c
long					ft_atol(const char *str);
void					clean(t_rules *rule);

// utilis_thread_mutex.c
int						handle_mutex(pthread_mutex_t *mutex, t_opcode opcode);
int						handle_thread_error(int status, t_opcode opcode);

// getters_setters.c
int						set_bool(pthread_mutex_t *mutex, bool *dest, \
	bool value);
int						get_bool(pthread_mutex_t *mutex, bool *value, \
	bool *val);
int						set_long(pthread_mutex_t *mutex, long *dest, \
	long value);
long					get_long(pthread_mutex_t *mutex, long *value);
int						simulation_finished(t_rules *rule, bool *out);

// dinner.c
int						start_dinner(t_rules *rule);
void					*dinner_simulation(void *data);

// create_and_join_threads.c
int						create_threads(t_rules *rule);
int						join_threads(t_rules *rule);

// philo_actions.c
int						think(t_philo *philo, bool pre_simulation);
void					*one_philo(void *arg);
int						eat(t_philo *philo);

// thread_utilis.c
int						smart_sleep(long duration, t_rules *rules);

// sync_utilis.c
int						wait_all_threads(t_rules *rule);
int						all_threads_running(pthread_mutex_t *mutex, \
	long *threads, long num_philos, bool *result);
int						increase_long(pthread_mutex_t *mutex, long *value);
int						de_synchronize_philo(t_philo *philo);

// utilis_timekeeper.c
long					gettime(t_time_code time_code);

// write.c
int						write_status(t_philo_status status, t_philo *philo, \
							bool debug);

// monitor.c
void					*monitor_dinner(void *data);

// debug.c
void					print_assigned_forks(t_rules *rules);