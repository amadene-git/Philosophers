/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:30:00 by admadene          #+#    #+#             */
/*   Updated: 2021/06/21 15:33:25 by admadene         ###   ########.fr       */
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
	int				is_die;
}				t_info;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_philo;
	pthread_mutex_t	mutex_fork;
	t_info			*info;
	struct timeval	last_meal;
	int				nbr_meal;
	pthread_t		thread_monito;
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
	if (gettimeofday(&(*info)->tv, &(*info)->tz))
		return (0);
//	(*info)->tzero = (*info)->tv;
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
	gettimeofday(&info->tv, &info->tz);
	printf("%ld%03.3d is start\n", \
	(info->tv.tv_sec - info->tzero.tv_sec), \
	info->tv.tv_usec / 1000);
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

long int	get_time_ms()
{
	struct	timeval tv;
	struct	timezone tz;
	long int ret;

	gettimeofday(&tv, &tz);
	ret = tv.tv_sec;
	ret *= 1000;
	ret += (tv.tv_usec/ 1000);
	return (ret);
}

void	philo_thinking(t_philo *philo)
{
	gettimeofday(&philo->info->tv, &philo->info->tz);
	if (!philo->info->is_die)
		printf("%ld%03.3d %d is thinking\n",\
		(philo->info->tv.tv_sec - philo->info->tzero.tv_sec), \
		philo->info->tv.tv_usec / 1000, philo->id);	
}

void	philo_take_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_fork);
	gettimeofday(&philo->info->tv, &philo->info->tz);
	if (!philo->info->is_die)
		printf("%ld%03.3d %d has taken a fork\n", \
		(philo->info->tv.tv_sec - philo->info->tzero.tv_sec), \
		philo->info->tv.tv_usec / 1000,  philo->id);
	pthread_mutex_lock(&(philo + left(philo->id, philo->info->nbr_philo))->mutex_fork);
	gettimeofday(&philo->info->tv, &philo->info->tz);
	if (!philo->info->is_die)
		printf("%ld%03.3d %d has taken a fork\n", \
		(philo->info->tv.tv_sec - philo->info->tzero.tv_sec), \
		philo->info->tv.tv_usec / 1000, philo->id);
}

void	philo_eating(t_philo *philo)
{
	gettimeofday(&philo->info->tv, &philo->info->tz);
	if (!philo->info->is_die)
		philo->last_meal = philo->info->tv;
	if (!philo->info->is_die)
	{
		printf("%ld%03.3d %d is eating\n", \
		(philo->info->tv.tv_sec - philo->info->tzero.tv_sec), \
		philo->info->tv.tv_usec / 1000, philo->id);
	}
	if (!philo->info->is_die)
		usleep(philo->info->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->mutex_fork);
	pthread_mutex_unlock(&(philo + left(philo->id, philo->info->nbr_philo))->mutex_fork);
}

void	philo_sleeping(t_philo *philo)
{
	gettimeofday(&philo->info->tv, &philo->info->tz);
	if (!philo->info->is_die)
	{
		printf("%ld%03.3d %d is sleeping\n", \
		(philo->info->tv.tv_sec - philo->info->tzero.tv_sec), \
		philo->info->tv.tv_usec / 1000, philo->id);
	}
	if (!philo->info->is_die)
		usleep(philo->info->time_to_sleep * 1000);
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
		gettimeofday(&(*philo)->info->tv, &(*philo)->info->tz);
		(*philo + i)->last_meal = (*philo)->info->tv;
		if (pthread_mutex_init(&(*philo + i)->mutex_fork, NULL))
			return (0);
		i++;
	}	
	return (1);
}

void *check_die(void *data)
{
	t_philo *philo;
	int i;

	philo = (t_philo *)data;
	while (1)
	{
		if (philo->info->is_die)
			return (NULL);
		gettimeofday(&philo->info->tv, &philo->info->tz);
		if (((philo->info->tv.tv_sec - philo->last_meal.tv_sec) * 1000) + ((philo->info->tv.tv_usec - philo->last_meal.tv_usec) / 1000) >= philo->info->time_to_die)
		{
			if (!philo->info->is_die)
			{
				philo->info->is_die = 1;
				printf("%ld%03.3d %d is dead\n", \
				(philo->info->tv.tv_sec - philo->info->tzero.tv_sec), \
				philo->info->tv.tv_usec / 1000, philo->id);
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
	
		gettimeofday(&philo->info->tv, &philo->info->tz);
	(*info).tzero = (*info).tv;
	printf("%ld%03.3d is start\n", \
	(info->tv.tv_sec - info->tzero.tv_sec), \
	info->tv.tv_usec / 1000);
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






















