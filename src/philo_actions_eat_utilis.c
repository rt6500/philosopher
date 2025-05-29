/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_eat_utilis.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:08:00 by rseki             #+#    #+#             */
/*   Updated: 2025/05/29 15:38:18 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	grab_forks(t_philo *philo)
{
	if (pthread_mutex_trylock(&philo->first_fork->fork) != 0)
  		printf("Philo %d couldn't take 1st fork\n", philo->id);
	// if (handle_mutex(&philo->first_fork->fork, LOCK))
	// 	return (1);
	if (write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE))
		return (1);
	if (pthread_mutex_trylock(&philo->second_fork->fork) != 0)
  		printf("Philo %d couldn't take 2nd fork\n", philo->id);
	// if (handle_mutex(&philo->second_fork->fork, LOCK))
	// 	return (1);
	if (write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE))
		return (1);
	return (0);
}

// returns -1 on error, 2 if already dead, 0 if ate
int start_eating(t_philo *p)
{
    t_rules *r = p->rules;
    long     now, last;

    if (pthread_mutex_lock(&p->philo_mutex) != 0)
        return -1;
    now  = gettime(MILLISECONDS);
    last = p->last_meal_time;

    if (now - last > r->time_to_die)
    {
        pthread_mutex_unlock(&p->philo_mutex);
        return 2;        // already dead
    }
    p->last_meal_time = now;
    if (pthread_mutex_unlock(&p->philo_mutex) != 0)
        return -1;

    if (write_status(EAT, p, DEBUG_MODE))
        return -1;
    smart_sleep(r->time_to_eat, p->rules);
    return 0;            // ate
}

// static int	start_eating(t_philo *philo)
// {
// 	long	now;

// 	if (handle_mutex(&philo->philo_mutex, LOCK))
// 		return (1);
// 	now = gettime(MILLISECONDS);
// 	if ((now - philo->last_meal_time) > philo->rules->time_to_die)
// 	{
// 		handle_mutex(&philo->philo_mutex, UNLOCK);
// 		return (2);
// 	}
// 	philo->last_meal_time = now;
// 	handle_mutex(&philo->philo_mutex, UNLOCK);
// 	philo->total_meals++;
// 	if (write_status(EAT, philo, DEBUG_MODE))
// 		return (1);
// 	if (smart_sleep(philo->rules->time_to_eat * 1000, philo->rules))
// 		return (1);
// 	return (0);
// }

static int	finish_eating(t_philo *philo)
{
	if (philo->rules->limit_meals > 0 \
		&& philo->total_meals == philo->rules->limit_meals)
	{
		if (set_bool(&philo->philo_mutex, &philo->full, true))
			return (1);
	}
	return (0);
}

static int	drop_forks(t_philo *philo)
{
	if (handle_mutex(&philo->first_fork->fork, UNLOCK))
		return (1);
	if (handle_mutex(&philo->second_fork->fork, UNLOCK))
		return (1);
	return (0);
}

// int	try_eating(t_philo *philo)
// {
// 	int	res;

// 	if (grab_forks(philo))
// 		return (1);
// 	res = start_eating(philo);	
// 	if (res == 1)
// 		return (drop_forks(philo), 1);
// 	if (res == 2)
// 		return (drop_forks(philo), 0);
// 	if (finish_eating(philo))
// 		return (drop_forks(philo), 1);
// 	if (drop_forks(philo))
// 		return (1);
// 	return (0);
// }

int	try_eating(t_philo *p)
{
	int	rc;
    if (grab_forks(p))
        return (-1);
    rc = start_eating(p);
    if (rc < 0)
    {
        drop_forks(p);
        return rc;     // -1 on error
    }
    if (rc == 2)
    {
        drop_forks(p);
        return 0;      // too late → didn’t eat
    }
    // rc == 0 → ate successfully
    finish_eating(p);
    drop_forks(p);
    return (1);
}
