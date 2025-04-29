/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utilis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:05:51 by rseki             #+#    #+#             */
/*   Updated: 2025/04/29 10:05:53 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
SPINLOCK until all threads get ready
*/
void    wait_all_threads(t_rules *rule)
{
    while (!get_bool(&rule->rule_mutex, &rule->all_threads_ready))
        ; // Do nothing while waiting.
}
