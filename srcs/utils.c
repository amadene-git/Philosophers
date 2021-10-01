/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 15:55:02 by admadene          #+#    #+#             */
/*   Updated: 2021/10/01 09:38:56 by admadene         ###   ########.fr       */
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
