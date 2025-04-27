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
typedef struct pthread_mutex_t	t_mtx;
typedef struct s_rules			t_rules;

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
}								t_opcode;

typedef struct s_fork
{
	pthread_mutex_t				fork;
	int							fork_id;
}								t_fork;

typedef struct s_philo
{
	int							id;
	pthread_mutex_t				*right_fork;
	pthread_mutex_t				*left_fork;
	pthread_t					thread;
	long						last_meal_time;
	long						total_meals;
	bool						full;
	t_rules						*rules;
}								t_philo;

typedef struct s_rules
{
	long						num_philos;
	long						time_to_die;
	long						time_to_eat;
	long						time_to_sleep;
	long						limit_meals;
	struct timeval				start_time;
	bool						end_simulation;
	t_fork						*forks;
	t_philo						*philos;
	pthread_mutex_t				red_flag;
	pthread_mutex_t				print_lock;
}								t_rules;

// main.c
int								main(int argc, char **argv);

// validate_input.c
int								is_number(const char *s);
int								is_valid_input(int argc, char **argv);
void							parse_input(char **argv, t_rules *rule);

// utilis.c
void							error_exit(const char *error);
void							how_long(t_rules rule);
void							free_array_philo(t_philo **array);
void							free_array_fork(t_fork **array);

// parse_input.c
void							parse_input(char **argv, t_rules *rule);
int								init_data(char **argv, t_rules *rule);

// utilis_thread_mutex.c
static int	handle_mutex_error(int status, t_opcode opcode);
int	handle_mutex(pthread_mutex_t *mutex, t_opcode opcode);
static int	handle_thread_error(int status, t_opcode opcode);
int	handle_thread(pthread_t *thread, void *(*foo)(void *), \
	void *data, t_opcode opcode);