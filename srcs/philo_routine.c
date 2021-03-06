/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 17:41:42 by admadene          #+#    #+#             */
/*   Updated: 2021/10/08 15:42:03 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_thinking(t_philo *philo)
{
	philo_print(philo->info->tzero, philo, "is thinking");
}

void	philo_take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo + left(philo->id, \
	philo->info->nbr_philo))->mutex_fork);
	philo_print(philo->info->tzero, philo, "has taken a fork");
	if (philo->info->nbr_philo == 1)
	{
		pthread_mutex_unlock(&(philo + left(philo->id, \
		philo->info->nbr_philo))->mutex_fork);
		ft_sleep(philo->info->time_to_die + 20, \
		&philo->info->is_die, get_time_us());
	}
	pthread_mutex_lock(&philo->mutex_fork);
	philo_print(philo->info->tzero, philo, "has taken a fork");
}

void	philo_eating(t_philo *philo)
{
	pthread_mutex_lock(&(philo->mutex_eat));
	if (!philo->info->is_die)
		philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&(philo->mutex_eat));
	philo_print(philo->info->tzero, philo, "is eating");
	ft_sleep(philo->info->time_to_eat, &philo->info->is_die, get_time_us());
	philo->nbr_meal++;
	pthread_mutex_unlock(&philo->mutex_fork);
	pthread_mutex_unlock(&(philo + left(philo->id, \
	philo->info->nbr_philo))->mutex_fork);
}

void	philo_sleeping(t_philo *philo)
{
	philo_print(philo->info->tzero, philo, "is sleeping");
	ft_sleep(philo->info->time_to_sleep, \
	&philo->info->is_die, get_time_us());
}

void	*routine_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	philo->info->i++;
	if (philo->info->i == philo->info->nbr_philo / 2)
		pthread_mutex_unlock(&philo->info->mutex_prio);
	while ((philo->info->each_must_eat == -1 || \
	philo->nbr_meal < philo->info->each_must_eat) && \
	!philo->info->is_die)
	{
		philo_thinking(philo);
		philo_take_fork(philo);
		philo_eating(philo);
		philo_sleeping(philo);
	}
	return (NULL);
}
