/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 16:36:55 by admadene          #+#    #+#             */
/*   Updated: 2021/09/19 16:06:17 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long int	get_time_ms(void)
{
	long int		ms;
	struct timeval	tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);
	ms = tv.tv_sec * 1000;
	ms += tv.tv_usec / 1000;
	return (ms);
}

long int	get_time_us(void)
{
	struct timeval	tv;
	struct timezone	tz;
	long int		time;

	gettimeofday(&tv, &tz);
	time = tv.tv_sec * 1000000;
	time += tv.tv_usec;
	return (time);
}

void	ft_sleep(long int ms, int *is_dead, long int tzero)
{
	while (get_time_us() - tzero < ms * 1000 && !*is_dead)
		usleep(200);
}
