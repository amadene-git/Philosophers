/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:45:33 by admadene          #+#    #+#             */
/*   Updated: 2021/10/01 09:38:01 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	die(t_philo *philo)
{
	long int i = get_time_ms() - philo->info->tzero;
		printf("%ld %d is die\n", i, philo->id + 1);
}

void	*check_die(void *data)
{
	t_philo	*philo;
	int		i;
	int		a;

	i = 0;
	a = 0;
	philo = (t_philo *)data;
	while (!philo->info->is_die)
	{
		if ((philo + i)->nbr_meal == philo->info->each_must_eat)
		{
			i++;
			a++;
			continue;
		}
		pthread_mutex_lock(&(philo + i)->mutex_eat);
		if (get_time_ms() - (philo + i)->last_meal / 1000 >= philo->info->time_to_die)
		{
			
			pthread_mutex_lock(&(philo->info->mutex_print));
			philo->info->is_die = 1;
			die(philo + i);
			pthread_mutex_unlock(&(philo->info->mutex_print));
			pthread_mutex_unlock(&(philo + i)->mutex_eat);
			return (NULL);
		}
		pthread_mutex_unlock(&(philo + i)->mutex_eat);
		if (i < philo->info->nbr_philo - 1)
			i++;
		else
		{
			if (a == i - 1)
				return (NULL);
			
			a = 0;
			i = 0;
			usleep(2000);
		}
	}
	return (NULL);
}

int	philo_birth(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&info->mutex_a);
	while (i < info->nbr_philo)
	{
		if (pthread_create(&(philo + i)->thread_philo, \
		NULL, routine_philo, philo + i))
			return (0);
		i += 2;
	}
	i = 1;
	if (info->nbr_philo > 1)
		pthread_mutex_lock(&info->mutex_a);
	usleep(200);
	while (i < info->nbr_philo)
	{
		if (pthread_create(&(philo + i)->thread_philo, \
		NULL, routine_philo, philo + i))
			return (0);
		i += 2;
	}
	pthread_mutex_unlock(&info->mutex_a);
	pthread_create(&info->thread_monito, NULL, check_die, philo);
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
	}
	pthread_join(philo->info->thread_monito, NULL);
	i = -1;
	while (++i < info->nbr_philo)
	{
		pthread_mutex_unlock(&(philo + i)->mutex_fork);
		pthread_mutex_unlock(&(philo + i)->mutex_eat);
		if (pthread_mutex_destroy(&(philo + i)->mutex_fork) ||\
		pthread_mutex_destroy(&(philo + i)->mutex_eat))
			return (0);
	}
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
	if (pthread_mutex_destroy(&(info->mutex_die)) || \
	pthread_mutex_destroy(&(info->mutex_print)) || \
	pthread_mutex_destroy(&(info->mutex_a)))
		return (0);
	free(philo);
	free(info);
	return (0);
}
