/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 17:31:50 by admadene          #+#    #+#             */
/*   Updated: 2021/10/01 11:05:12 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	right(int id, int nb_philo)
{
	if (id == nb_philo - 1)
		return (-id);
	return (1);
}

void	philo_thinking(t_philo *philo)
{
	philo_print(philo->info->tzero, philo, \
	"is thinking");
/*	while (!philo->info->is_die && (get_time_us() - philo->last_meal) \
	/ 1000 < philo->info->time_to_die / 3 && \
	((philo->nbr_meal > philo->info->j / philo->info->nbr_philo) || \
	((philo + left(philo->id, philo->info->nbr_philo))->last_meal \
	< philo->last_meal || (philo + right(philo->id, \
	philo->info->nbr_philo))->last_meal < philo->last_meal)))
		usleep(200);
*/
	while (!philo->info->is_die && ((philo->info->j + philo->info->nbr_philo)  / philo->info->nbr_philo / 2) % 2 != philo->id % 2)
		usleep(200);
	printf("philo j %d\n", philo->info->j);
}

void	philo_take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo + left(philo->id, \
	philo->info->nbr_philo))->mutex_fork);
	philo_print(philo->info->tzero, philo, \
	"has taken a fork");
	if (philo->info->nbr_philo == 1)
	{
		pthread_mutex_unlock(&(philo + left(philo->id, \
		philo->info->nbr_philo))->mutex_fork);
		ft_sleep(philo->info->time_to_die * 2, \
		&philo->info->is_die, get_time_us());
		return ;
	}
	if (philo->info->is_die)
	{
		pthread_mutex_unlock(&(philo + left(philo->id, \
		philo->info->nbr_philo))->mutex_fork);
		return ;
	}
	pthread_mutex_lock(&philo->mutex_fork);
	philo_print(philo->info->tzero, philo, \
	"has taken a fork");
}

void	philo_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_eat);
	if (!philo->info->is_die)
		philo->last_meal = get_time_us();
	pthread_mutex_unlock(&philo->mutex_eat);
	philo_print(philo->info->tzero, philo, \
	"is eating");
	if (!philo->info->is_die)
		ft_sleep(philo->info->time_to_eat, &philo->info->is_die, get_time_us());
	philo->nbr_meal++;
	pthread_mutex_unlock(&philo->mutex_fork);
	pthread_mutex_unlock(&(philo + left(philo->id, \
	philo->info->nbr_philo))->mutex_fork);
}

void	philo_sleeping(t_philo *philo)
{
	philo_print(philo->info->tzero, philo, \
	"is sleeping");
	if (!philo->info->is_die)
		ft_sleep(philo->info->time_to_sleep, \
		&philo->info->is_die, get_time_us());
}

int	mutex_protection(t_philo *philo)
{
	if (philo->info->is_die)
	{
		pthread_mutex_unlock(&philo->mutex_fork);
		pthread_mutex_unlock(&(philo + left(philo->id, \
		philo->info->nbr_philo))->mutex_fork);
		return (0);
	}
	return (1);
}

void	*routine_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	philo->info->i++;
	if (philo->info->i == philo->info->nbr_philo / 2)
		pthread_mutex_unlock(&philo->info->mutex_a);
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
		if (!mutex_protection(philo))
			return (NULL);
		philo_eating(philo);
		philo->info->j++;
		if (philo->info->is_die)
			return (NULL);
		philo_sleeping(philo);
	}
	return (NULL);
}
