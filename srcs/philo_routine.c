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
}

void	philo_take_fork(t_philo *philo)
{
	// printf("fork -> %d | 1\n", philo->id + 1);
	pthread_mutex_lock(&(philo + left(philo->id, \
	philo->info->nbr_philo))->mutex_fork);
	philo_print(philo->info->tzero, philo, \
	"has taken a fork");
	// printf("fork -> %d | 2\n", philo->id + 1);
	if (philo->info->nbr_philo == 1)
	{
	// printf("fork -> %d | 3\n", philo->id + 1);
		pthread_mutex_unlock(&(philo + left(philo->id, \
		philo->info->nbr_philo))->mutex_fork);
	// printf("fork -> %d | 4\n", philo->id + 1);
		ft_sleep(philo->info->time_to_die * 2, \
		&philo->info->is_die, get_time_us());
		return ;
	}
	// printf("fork -> %d | 5\n", philo->id + 1);
	if (philo->info->is_die)
	{
	// printf("fork -> %d | 6\n", philo->id + 1);
		pthread_mutex_unlock(&(philo + left(philo->id, \
		philo->info->nbr_philo))->mutex_fork);
	// printf("fork -> %d | 7\n", philo->id + 1);
		return ;
	}
	// printf("fork -> %d | 8\n", philo->id + 1);
	pthread_mutex_lock(&philo->mutex_fork);
	philo_print(philo->info->tzero, philo, \
	"has taken a fork");
	// printf("fork -> %d | 10\n", philo->id + 1);

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

void	*routine_philo_odd(void *data)
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
		pthread_mutex_lock(&philo->info->mutex_odd);
		pthread_mutex_unlock(&philo->info->mutex_odd);
		if (philo->info->is_die)
			return (NULL);
		// printf("philo-> %d | 4\n", philo->id + 1);
		philo_take_fork(philo);
		philo->info->j++;
		if (philo->info->j == philo->info->nbr_philo / 2)
		{
			philo->info->j = 0;
			pthread_mutex_unlock(&philo->info->mutex_even);
			pthread_mutex_lock(&philo->info->mutex_odd);
		}
		// printf("philo-> %d | 5\n", philo->id + 1);
		if (!mutex_protection(philo))
			return (NULL);
		// printf("philo-> %d | 6\n", philo->id + 1);
		philo_eating(philo);
		// printf("philo-> %d | 7\n", philo->id + 1);
		if (philo->info->is_die)
			return (NULL);
		// printf("philo-> %d | 8\n", philo->id + 1);
		philo_sleeping(philo);
		// printf("philo-> %d | 9\n", philo->id + 1);
	}
	printf("philo->%d out\n", philo->id + 1);
	return (NULL);
}

void	*routine_philo_even(void *data)
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
		pthread_mutex_lock(&philo->info->mutex_even);
		pthread_mutex_unlock(&philo->info->mutex_even);
		if (philo->info->is_die)
			return (NULL);
		// printf("philo-> %d | 4\n", philo->id + 1);
		philo_take_fork(philo);
		philo->info->j++;
		if (philo->info->j == philo->info->nbr_philo / 2)
		{
			philo->info->j = 0;
			pthread_mutex_unlock(&philo->info->mutex_odd);
			pthread_mutex_lock(&philo->info->mutex_even);
		}
		// printf("philo-> %d | 5\n", philo->id + 1);
		if (!mutex_protection(philo))
			return (NULL);
		// printf("philo-> %d | 6\n", philo->id + 1);
		philo_eating(philo);
		// printf("philo-> %d | 7\n", philo->id + 1);
		if (philo->info->is_die)
			return (NULL);
		// printf("philo-> %d | 8\n", philo->id + 1);
		philo_sleeping(philo);
		// printf("philo-> %d | 9\n", philo->id + 1);
	}
	return (NULL);
}
