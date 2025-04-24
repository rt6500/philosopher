/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rseki <rseki@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:56:05 by rseki             #+#    #+#             */
/*   Updated: 2025/04/24 10:56:06 by rseki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	is_number(const char *s)
{
	long long int	result;
	int				i;

	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	result = 0;
	while (s[i])
	{
		if (s[i] < '0' || '9' < s[i])
			return (0);
		if (result > (LLONG_MAX - (s[i] - '0')) / 10)
			return (0);
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (1);
}

int	is_valid_input(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc == 4 || argc == 5)
	{
		while (i < argc)
		{
			if (argv[i][0] == '-')
				return (0);
			if (argv[i][0] == '+')
				i++;
			if (!is_number(argv[i]))
				return (0);
			i++;
		}
		return (1);
	}
	else
		return (0);
}
