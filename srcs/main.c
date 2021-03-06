/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:45:33 by admadene          #+#    #+#             */
/*   Updated: 2021/10/08 15:44:14 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	die(t_philo *philo)
{
	long int	i;

	pthread_mutex_lock(&philo->mutex_eat);
	if (get_time_ms() - philo->last_meal \
	>= philo->info->time_to_die)
	{
		pthread_mutex_lock(&philo->info->mutex_print);
		philo->info->is_die = 1;
		i = get_time_ms() - philo->info->tzero;
		printf("%ld %d is died\n", i, philo->id + 1);
		pthread_mutex_unlock(&philo->mutex_fork);
		pthread_mutex_unlock(&(philo + left(philo->id, \
		philo->info->nbr_philo))->mutex_fork);
		pthread_mutex_unlock(&philo->info->mutex_print);
		pthread_mutex_unlock(&philo->mutex_eat);
		return (0);
	}
	pthread_mutex_unlock(&philo->mutex_eat);
	return (1);
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
		if (i < philo->info->nbr_philo && (philo + i)->nbr_meal == philo->info->each_must_eat)
		{
			i++;
			a++;
			if (a == philo->info->nbr_philo)
				return (NULL);
			continue ;
		}
		else if (i < philo->info->nbr_philo && !die(philo + i))
			return (NULL);
		if (i < philo->info->nbr_philo - 1)
			i++;
		else if (!die_2(&i, &a))
			return (NULL);
	}
	return (NULL);
}

int	philo_birth(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&info->mutex_prio);
	while (i < info->nbr_philo)
	{
		pthread_create(&(philo + i)->thread_philo, \
		NULL, routine_philo, philo + i);
		i += 2;
	}
	if (info->nbr_philo > 1)
		pthread_mutex_lock(&info->mutex_prio);
	usleep(600);
	i = 1;
	while (i < info->nbr_philo)
	{
		pthread_create(&(philo + i)->thread_philo, \
		NULL, routine_philo, philo + i);
		i += 2;
	}
	pthread_mutex_unlock(&info->mutex_prio);
	if (pthread_create(&info->thread_monito, NULL, check_die, philo))
		return (0);
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
		if (pthread_mutex_destroy(&(philo + i)->mutex_fork) \
		 || pthread_mutex_destroy(&(philo + i)->mutex_eat))
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
	pthread_mutex_destroy(&(info->mutex_die));
	pthread_mutex_destroy(&(info->mutex_print));
	pthread_mutex_destroy(&(info->mutex_prio));
	free(philo);
	free(info);
	return (0);
}
