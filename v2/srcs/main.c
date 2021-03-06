/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:45:33 by admadene          #+#    #+#             */
/*   Updated: 2021/07/01 19:46:50 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

long int	ft_atoli(char *str)
{
	long long int	nbr;
	int				neg;

	neg = 1;
	nbr = 0;
	while (*str == ' ')
		str++;
	if (*str == '-' && str++)
		neg = -1;
	while (ft_is_digit(*str))
	{
		nbr += (*str - '0') * neg;
		nbr *= 10;
		str++;
	}
	while (*str == ' ')
		str++;
	if (*str)
		return (-1);
	nbr /= 10;
	return (nbr);
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
		
		pthread_mutex_lock(&philo->mutex_eat);
		if ((get_time_ms() - philo->last_meal >= philo->info->time_to_die && \
		(philo->nbr_meal < philo->info->each_must_eat \
		|| philo->info->each_must_eat == -1)))
		{
			if (!philo->info->is_die)
			{
				philo->info->is_die = 1;
				printf("%ld %d is died\n", \
				get_time_ms() - philo->info->tzero, philo->id);
			}
			return (NULL);
		}
		pthread_mutex_unlock(&philo->mutex_eat);
		usleep(5000);
	}
	return (NULL);
}

int	philo_life(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	(*info).tzero = get_time_ms();
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
	printf("1\n");
	i = -1;
	while (++i < info->nbr_philo)
	{
		printf("2\n");
		pthread_join((philo + i)->thread_philo, NULL);
		printf("2\n");
		pthread_join((philo + i)->thread_monito, NULL);
		printf("2\n");
	}
	printf("1\n");
	i = -1;
    pthread_mutex_destroy(&(info->mutex_die));
	while (++i < info->nbr_philo)
		if (pthread_mutex_destroy(&(philo + i)->mutex_fork))
			return (0);
	printf("1\n");
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
	free(philo);
	free(info);
	return (0);
}
