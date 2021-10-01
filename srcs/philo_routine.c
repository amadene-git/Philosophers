/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 17:31:50 by admadene          #+#    #+#             */
/*   Updated: 2021/09/25 02:32:00 by admadene         ###   ########.fr       */
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
	while ((philo->nbr_meal > philo->info->j / philo->info->nbr_philo))
		ft_sleep(philo->info->time_to_eat / 4, &philo->info->is_die, get_time_us());
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
	printf("1-philo %d eating -> %p\n", philo->id + 1, &philo->mutex_eat);
	pthread_mutex_lock(&philo->mutex_eat);
	printf("2-philo %d eating\n", philo->id + 1);
	if (!philo->info->is_die)
		philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->mutex_eat);
	philo_print(philo->info->tzero, philo, \
	"is eating");
	printf("philo %d die unlock\n", philo->id + 1);
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
		printf("philo %d fork\n", philo->id + 1);
		if (philo->info->is_die)
		{
			printf("1-philo %d force unlock\n", philo->id + 1);	
			pthread_mutex_unlock(&philo->mutex_fork);
			pthread_mutex_unlock(&(philo + left(philo->id, \
			philo->info->nbr_philo))->mutex_fork);
			printf("2-philo %d force unlock\n", philo->id + 1);	
			return (NULL);
		}
		printf("1-philo %d eat\n", philo->id + 1);
		philo_eating(philo);
		printf("2-philo %d eat\n", philo->id + 1);
		philo->info->j++;
		if (philo->info->is_die)
			return (NULL);
		philo_sleeping(philo);
		if (philo->info->is_die)
			return (NULL);
	}
//	printf("philo %d has stopped\n", philo->id + 1);
	return (NULL);
}
