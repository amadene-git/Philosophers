/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 17:39:10 by admadene          #+#    #+#             */
/*   Updated: 2021/09/24 15:52:35 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_nb(long int nb)
{
	if (nb >= 0 && nb <= 2147483647)
		return (1);
	return (0);
}

int	init_info(char **av, t_info **info)
{
	*info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (0);
	(*info)->each_must_eat = -1;
	(*info)->nbr_philo = ft_atoli(*(av++));
	(*info)->time_to_die = ft_atoli(*(av++));
	(*info)->time_to_eat = ft_atoli(*(av++));
	(*info)->time_to_sleep = ft_atoli(*(av++));
	(*info)->is_die = 0;
	if (!check_nb((*info)->nbr_philo) || !check_nb((*info)->time_to_die) \
	|| !check_nb((*info)->time_to_eat) || !check_nb((*info)->time_to_sleep) \
	|| !(*info)->nbr_philo)
		return (0);
	if (*av)
	{
		(*info)->each_must_eat = ft_atoli(*av);
		if (!check_nb((*info)->each_must_eat))
			return (0);
	}
	if (pthread_mutex_init(&((*info)->mutex_die), NULL) \
	|| pthread_mutex_init(&((*info)->mutex_print), NULL))
		return (0);
	return (1);
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

void	philo_print(long int tzero, t_philo *philo, const char *str)
{
	pthread_mutex_lock(&philo->info->mutex_print);
	if (!philo->info->is_die)
		printf("%ld %d %s\n", get_time_ms() - tzero, \
		philo->id + 1, str);
	pthread_mutex_unlock(&philo->info->mutex_print);
}
