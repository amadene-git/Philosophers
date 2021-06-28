/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 17:31:50 by admadene          #+#    #+#             */
/*   Updated: 2021/06/22 17:34:08 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_thinking(t_philo *philo)
{
	philo->info->time = get_time_ms();
	if (!philo->info->is_die)
		printf("%ld %d is thinking\n", \
		get_time_ms() - philo->info->tzero, philo->id);
}

void	philo_take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo + left(philo->id, \
	philo->info->nbr_philo))->mutex_fork);
	if (!philo->info->is_die)
		printf("%ld %d has taken a fork\n", \
		get_time_ms() - philo->info->tzero, philo->id);
	pthread_mutex_lock(&philo->mutex_fork);
	if (!philo->info->is_die)
		printf("%ld %d has taken a fork\n", \
		get_time_ms() - philo->info->tzero, philo->id);
}

void	philo_eating(t_philo *philo)
{
	if (!philo->info->is_die)
		philo->last_meal = get_time_ms();
	if (!philo->info->is_die)
		printf("%ld %d is eating\n", \
		get_time_ms() - philo->info->tzero, philo->id);
	if (!philo->info->is_die)
		ft_sleep(philo->info->time_to_eat, &philo->info->is_die, get_time_us());
	philo->nbr_meal++;
	pthread_mutex_unlock(&philo->mutex_fork);
	pthread_mutex_unlock(&(philo + left(philo->id, \
	philo->info->nbr_philo))->mutex_fork);
}

void	philo_sleeping(t_philo *philo)
{
	if (!philo->info->is_die)
		printf("%ld %d is sleeping\n", \
		get_time_ms() - philo->info->tzero, philo->id);
	if (!philo->info->is_die)
		ft_sleep(philo->info->time_to_sleep, &philo->info->is_die, get_time_us());
}

void	*routine_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while ((philo->info->each_must_eat == -1 || \
	philo->nbr_meal < philo->info->each_must_eat) && \
	!philo->info->is_die)
	{
		if (philo->info->is_die)
			return (NULL);
		philo_thinking(philo);
		if (philo->info->is_die)
			return (NULL);
		philo_take_fork(philo);
		philo_eating(philo);
		if (philo->info->is_die)
			return (NULL);
		philo_sleeping(philo);
		if (philo->info->is_die)
			return (NULL);
	}
	return (NULL);
}
