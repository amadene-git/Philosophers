/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:45:33 by admadene          #+#    #+#             */
/*   Updated: 2021/06/22 14:59:36 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *str)
{
	long int	nbr;
	int			neg;

	neg = 1;
	nbr = 0;
	while (*str == ' ')
		str++;
	if (*str == '-' && str++)
		neg = -1;
	while (*str >= '0' && *str <= '9')
	{
		nbr += (*str - '0') * neg;
		nbr *= 10;
		str++;
	}
	nbr /= 10;
	return (nbr);
}

int	init_info(char **av, t_info **info)
{
	*info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (0);
	(*info)->each_must_eat = -1;
	(*info)->nbr_philo = ft_atoi(*(av++));
	(*info)->time_to_die = ft_atoi(*(av++));
	(*info)->time_to_eat = ft_atoi(*(av++));
	(*info)->time_to_sleep = ft_atoi(*(av++));
	(*info)->is_die = 0;
	if ((*info)->nbr_philo < 0 || (*info)->time_to_die < 0 \
			|| (*info)->time_to_eat < 0 || (*info)->time_to_sleep < 0)
		return (0);
	if (*av)
	{
		(*info)->each_must_eat = ft_atoi(*av);
		if ((*info)->each_must_eat < 0)
			return (0);
	}
	return (1);
}

void	print_philo_info(t_info *info)
{
	if (!info)
		return ;
	printf("nbr philo %d\n", info->nbr_philo);
	printf("time to die %d\n", info->time_to_die);
	printf("time to eat %d\n", info->time_to_eat);
	printf("time to sleep %d\n", info->time_to_sleep);
	printf("each must eat %d\n", info->each_must_eat);
	printf("%ld is start\n", get_time_ms() - info->tzero);
}

int		right(int index, int max)
{
	if (index < max)
		return (1);
	return ((max - 1) * -1);
}

int		left(int index, int max)
{
	if (index)
		return (-1);
	return (max - 1);
}

void	philo_thinking(t_philo *philo)
{
	philo->info->time = get_time_ms();
	if (!philo->info->is_die)
		printf("%ld %d is thinking\n", get_time_ms() - philo->info->tzero, philo->id);
}

void	philo_take_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_fork);
	if (!philo->info->is_die)
		printf("%ld %d has taken a fork\n", get_time_ms() - philo->info->tzero, philo->id);
	pthread_mutex_lock(&(philo + left(philo->id, philo->info->nbr_philo))->mutex_fork);
	if (!philo->info->is_die)
		printf("%ld %d has taken a fork\n", get_time_ms() - philo->info->tzero, philo->id);
}

void	philo_eating(t_philo *philo)
{
	if (!philo->info->is_die)
		philo->last_meal = get_time_ms();
	if (!philo->info->is_die)
	{
		printf("%ld %d is eating\n", get_time_ms() - philo->info->tzero, philo->id);
	}
	if (!philo->info->is_die)
		ft_sleep(philo->info->time_to_eat, &philo->info->is_die);
	pthread_mutex_unlock(&philo->mutex_fork);
	pthread_mutex_unlock(&(philo + left(philo->id, philo->info->nbr_philo))->mutex_fork);
}

void	philo_sleeping(t_philo *philo)
{
	if (!philo->info->is_die)
	{
		printf("%ld %d is sleeping\n", get_time_ms() - philo->info->tzero, philo->id);
	}
	if (!philo->info->is_die)
		ft_sleep(philo->info->time_to_sleep, &philo->info->is_die);
}

void	*routine_philo(void *data)
{
	t_philo *philo = (t_philo *)data;

	while ((philo->info->each_must_eat == -1 || philo->nbr_meal < philo->info->each_must_eat) && \
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
		philo->nbr_meal++;
	}

	return (NULL);
}

int		init_philo(t_philo **philo, t_info *info)
{
	int i;

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

void *check_die(void *data)
{
	t_philo *philo;
	int i = 0;

	philo = (t_philo *)data;
	while (1)
	{
		if (philo->info->is_die)
			return (NULL);
		if (get_time_ms() - philo->last_meal >= philo->info->time_to_die)
		{
			usleep(100);
			if (!philo->info->is_die)
			{
				philo->info->is_die = 1;
				printf("%ld %d is died\n", get_time_ms() - philo->info->tzero, philo->id);
			}
			return (NULL);
		}
		i++;
	}
}

int	philo_life(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	(*info).tzero = get_time_ms();
	while (i < info->nbr_philo)
	{
		if (pthread_create(&(philo + i)->thread_philo, NULL, routine_philo, philo + i) ||
			pthread_create(&(philo + i)->thread_monito, NULL, check_die, philo + i))
			return (0);
		i++;
	}
	i = -1;
	while (++i < info->nbr_philo)
	{
		pthread_join((philo + i)->thread_philo, NULL);
		pthread_join((philo + i)->thread_monito, NULL);

	}
/*	i = -1;
	while (++i < info->nbr_philo)
	{
	
	}*/
	return (0);
}

int	main(const int ac, char **av)
{
	t_info	*info;
	t_philo	*philo;

	if (ac < 5 || ac > 6 || !init_info(av + 1, &info))
	{
		write(2, "Error bad arguments\n", 20);
		return (-1);
	}
	print_philo_info(info);
	if (!init_philo(&philo, info))
		return (-1);
	philo_life(philo, info);
	check_die(philo);
	return (0);
}
