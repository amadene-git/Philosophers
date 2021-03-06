/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 15:55:02 by admadene          #+#    #+#             */
/*   Updated: 2021/10/08 15:46:18 by admadene         ###   ########.fr       */
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

int	die_2(int *i, int *a)
{
	if (*a == *i - 1)
		return (0);
	*a = 0;
	*i = 0;
	usleep(1000);
	return (1);
}

int	check_nb(long int nb)
{
	if (nb >= 0 && nb <= 2147483647)
		return (1);
	return (0);
}

int	left(int index, int max)
{
	if (index)
		return (-1);
	return (max - 1);
}
