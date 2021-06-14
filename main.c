/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 10:53:11 by admadene          #+#    #+#             */
/*   Updated: 2021/06/14 17:20:09 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_info
{	
	struct timeval	tzero;
	struct timeval	tv;
	struct timezone	tz;
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				each_must_eat;
}				t_info;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_philo;
	pthread_mutex_t	mutex_fork;
	t_info			*info;
	suseconds_t		last_meal;
	int				nbr_meal;
}				t_philo;

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
	if ((*info)->nbr_philo < 0 || (*info)->time_to_die < 0 \
			|| (*info)->time_to_eat < 0 || (*info)->time_to_sleep < 0)
		return (0);
	if (*av)
	{
		(*info)->each_must_eat = ft_atoi(*av);
		if ((*info)->each_must_eat < 0)
			return (0);
	}
	if (gettimeofday(&(*info)->tv, &(*info)->tz))
		return (0);
	(*info)->tzero = (*info)->tv;
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
	printf("Tv = %ld %d, T0 = %d [%d]\n", info->tv.tv_sec, info->tv.tv_usec, info->tv.tv_usec - info->tzero.tv_usec, info->tzero.tv_usec);
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

int		getrandom()
{
	struct	timeval tv;
	struct	timezone tz;
	gettimeofday(&tv, &tz);
	return (tv.tv_usec % 10);

}

void	*routine_philo(void *data)
{
	t_philo *philo = (t_philo *)data;

	while (1)
	{	
		gettimeofday(&philo->info->tv, &philo->info->tz);
		philo->last_meal = philo->info->tv.tv_usec;

		printf("%ld%d %d is thinking\n", (philo->info->tv.tv_sec - philo->info->tzero.tv_sec), philo->info->tv.tv_usec / 1000, philo->id);
		pthread_mutex_lock(&philo->mutex_fork);

		gettimeofday(&philo->info->tv, &philo->info->tz);
		//printf("%d %d has taken a fork\n", (philo->info->tv.tv_usec - philo->info->tzero) / 1000, philo->id);
		pthread_mutex_lock(&(philo + left(philo->id, philo->info->nbr_philo))->mutex_fork);

		gettimeofday(&philo->info->tv, &philo->info->tz);
		//printf("%d %d has taken a fork\n", (philo->info->tv.tv_usec - philo->info->tzero) / 1000, philo->id);

		gettimeofday(&philo->info->tv, &philo->info->tz);
		philo->last_meal = philo->info->tv.tv_usec;
		//printf("%d %d is eating\n", (philo->info->tv.tv_usec - philo->info->tzero) / 1000, philo->id);
		usleep(philo->info->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->mutex_fork);
		pthread_mutex_unlock(&(philo + left(philo->id, philo->info->nbr_philo))->mutex_fork);


		gettimeofday(&philo->info->tv, &philo->info->tz);
		//printf("%d %d is sleeping\n", (philo->info->tv.tv_usec - philo->info->tzero) / 1000, philo->id);
		usleep(philo->info->time_to_sleep * 1000);
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
		if (pthread_mutex_init(&(*philo + i)->mutex_fork, NULL))
			return (0);
		i++;
	}	
	return (1);
}

int	philo_life(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->nbr_philo)
	{
		if (pthread_create(&(philo + i)->thread_philo, NULL, routine_philo, philo + i))
			return (0);
		i++;
	}
	i = -1;
	while (++i < info->nbr_philo)
		pthread_join((philo + i)->thread_philo, NULL);
	return (0);
}

/*
   void	*check_philo(void *data)
   {
   while ()


   }
   */

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
	return (0);
}






















