/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 16:36:55 by admadene          #+#    #+#             */
/*   Updated: 2021/06/30 11:08:48 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long int	get_time_ms(void)
{
    struct timeval	tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long int	get_time_us(void)
{
    struct timeval	tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000 + tv.tv_usec);
}

int         get_random(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_usec % 10);
}

void	ft_sleep(long int ms, int *is_dead, long int tzero)
{
    int			i;
    int			a;

    i = 0;
    a = 0;
    while (++i < ms)
    {
        if (*is_dead || ms * 1000 <= get_time_us() - tzero)
        {
            return ;
        }
        if (a < 1000)
        {
            usleep(1000 - a);
            a = ((get_time_us() - tzero) - 1000 * i);
        }
        else
            a -= 1000;
    }
        usleep(get_random() * 100);
}
