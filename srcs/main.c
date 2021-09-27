/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:45:33 by admadene          #+#    #+#             */
/*   Updated: 2021/09/25 00:49:13 by admadene         ###   ########.fr       */
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
	while (!philo->info->is_die)
	{
		if (philo->nbr_meal == philo->info->each_must_eat)
		{
			i++;
			continue;
		}
		if (get_time_ms() - philo->last_meal > philo->info->time_to_die)
		{
			die(philo);
			return (NULL);
		}
		if (i <= philo->info->nbr_philo)
			i++;
		else
		{
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
	pthread_mutex_lock(&info->mutex_a);
	//printf("ici\n");
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
		if (pthread_mutex_destroy(&(philo + i)->mutex_fork))
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
	//check_die(philo);
	if (pthread_mutex_destroy(&(info->mutex_die)) || \
	pthread_mutex_destroy(&(info->mutex_print)) || \
	pthread_mutex_destroy(&(info->mutex_a)))
		return (0);
	free(philo);
	free(info);
	return (0);
}
