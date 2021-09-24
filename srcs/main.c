/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:45:33 by admadene          #+#    #+#             */
/*   Updated: 2021/09/24 15:57:39 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	die(t_philo *philo)
{
	pthread_mutex_lock(&(philo->info->mutex_die));
	if (!philo->info->is_die)
	{
		pthread_mutex_lock(&philo->info->mutex_print);
		philo->info->is_die = 1;
		printf("%ld %d is die\n", get_time_ms() - philo->info->tzero, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_print);
	}
	pthread_mutex_unlock(&(philo->info->mutex_die));
}

void	*check_die(void *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = (t_philo *)data;
	while (philo->nbr_meal < philo->info->each_must_eat \
			|| philo->info->each_must_eat == -1)
	{
		if (philo->info->is_die)
			return (NULL);
		if ((get_time_ms() - philo->last_meal > philo->info->time_to_die && \
		(philo->nbr_meal < philo->info->each_must_eat \
		|| philo->info->each_must_eat == -1)))
		{
			die(philo);
			return (NULL);
		}
		usleep(2000);
	}
	return (NULL);
}

int	philo_birth(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->nbr_philo)
	{
		if (pthread_create(&(philo + i)->thread_philo, \
		NULL, routine_philo, philo + i) || \
		pthread_create(&(philo + i)->thread_monito, NULL, check_die, philo + i))
			return (0);
		i += 2;
	}
	usleep(100);
	i = 1;
	while (i < info->nbr_philo)
	{
		if (pthread_create(&(philo + i)->thread_philo, \
		NULL, routine_philo, philo + i) || \
		pthread_create(&(philo + i)->thread_monito, NULL, check_die, philo + i))
			return (0);
		i += 2;
	}
	return (1);
}

int	philo_life(t_philo *philo, t_info *info)
{
	int	i;

	(*info).tzero = get_time_ms();
	if (!philo_birth(philo, info))
		return (0);
	i = -1;
	while (++i < info->nbr_philo)
	{
		pthread_join((philo + i)->thread_philo, NULL);
		pthread_join((philo + i)->thread_monito, NULL);
	}
	i = -1;
	while (++i < info->nbr_philo)
		if (pthread_mutex_destroy(&(philo + i)->mutex_fork))
			return (0);
	return (0);
}

int	main(const int ac, char **av)
{
	t_info	*info;
	t_philo	*philo;

	info = NULL;
	philo = NULL;
	if (ac < 5 || ac > 6 || !init_info(av + 1, &info))
	{
		write(2, "Error bad arguments\n", 20);
		free(info);
		return (-1);
	}
	if (!init_philo(&philo, info))
	{
		free(info);
		free(philo);
		return (-1);
	}
	philo_life(philo, info);
	check_die(philo);
	if (pthread_mutex_destroy(&(info->mutex_die)) || \
	pthread_mutex_destroy(&(info->mutex_print)))
		return (0);
	free(philo);
	free(info);
	return (0);
}
