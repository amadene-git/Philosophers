/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:46:31 by admadene          #+#    #+#             */
/*   Updated: 2021/06/28 15:35:02 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_info
{
	long int		tzero;
	long int		time;
	long int		nbr_philo;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		each_must_eat;
	int				is_die;
}				t_info;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_philo;
	pthread_mutex_t	mutex_fork;
	t_info			*info;
	long int		last_meal;
	int				nbr_meal;
	pthread_t		thread_monito;
}				t_philo;

long int	get_time_ms(void);
long int	get_time_us(void);
void		ft_sleep(long int ms, int *is_dead, long int tzero);
void		philo_thinking(t_philo *philo);
void		philo_take_fork(t_philo *philo);
void		philo_eating(t_philo *philo);
void		philo_sleeping(t_philo *philo);
void		*routine_philo(void *data);
long int	ft_atoli(char *str);
int			init_info(char **av, t_info **info);
int			right(int index, int max);
int			left(int index, int max);
int			init_philo(t_philo **philo, t_info *info);
int			philo_life(t_philo *philo, t_info *info);
void		*check_die(void *data);
#endif
