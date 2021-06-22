/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 15:46:31 by admadene          #+#    #+#             */
/*   Updated: 2021/06/22 14:55:23 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_info
{
	long int		tzero;
	long int 		time;
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				each_must_eat;
	int				is_die;
}				t_info;

typedef struct	s_philo
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
void		ft_sleep(long int ms, int *is_dead);

#endif
