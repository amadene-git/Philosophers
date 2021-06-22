/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 17:39:10 by admadene          #+#    #+#             */
/*   Updated: 2021/06/22 17:41:23 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *str)
{
	long int	nbr;
	int			neg;

	neg = 1;
	nbr = 0;
	while (*str == ' ')
		str++;
	if (*str == '-' && str++)
		neg = -1;
	while (*str >= '0' && *str <= '9')
	{
		nbr += (*str - '0') * neg;
		nbr *= 10;
		str++;
	}
	nbr /= 10;
	return (nbr);
}

int	init_info(char **av, t_info **info)
{
	*info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (0);
	(*info)->each_must_eat = -1;
	(*info)->nbr_philo = ft_atoi(*(av++));
	(*info)->time_to_die = ft_atoi(*(av++));
	(*info)->time_to_eat = ft_atoi(*(av++));
	(*info)->time_to_sleep = ft_atoi(*(av++));
	(*info)->is_die = 0;
	if ((*info)->nbr_philo < 0 || (*info)->time_to_die < 0 \
			|| (*info)->time_to_eat < 0 || (*info)->time_to_sleep < 0)
		return (0);
	if (*av)
	{
		(*info)->each_must_eat = ft_atoi(*av);
		if ((*info)->each_must_eat < 0)
			return (0);
	}
	return (1);
}

int	right(int index, int max)
{
	if (index < max)
		return (1);
	return ((max - 1) * -1);
}

int	left(int index, int max)
{
	if (index)
		return (-1);
	return (max - 1);
}

int	init_philo(t_philo **philo, t_info *info)
{
	int	i;

	*philo = (t_philo *)malloc(sizeof(t_philo) * info->nbr_philo);
	if (!*philo)
		return (0);
	i = 0;
	while (i < info->nbr_philo)
	{
		(*philo + i)->info = info;
		(*philo + i)->id = i;
		(*philo + i)->nbr_meal = 0;
		(*philo + i)->last_meal = get_time_ms();
		if (pthread_mutex_init(&(*philo + i)->mutex_fork, NULL))
			return (0);
		i++;
	}
	return (1);
}
