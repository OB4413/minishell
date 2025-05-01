/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 22:27:40 by obarais           #+#    #+#             */
/*   Updated: 2024/11/05 12:34:47 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	size_t	i;
	int		a;
	long	num;

	i = 0;
	a = 1;
	num = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			a = a * (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (num > (LONG_MAX - (str[i] - 48)) / 10)
			return (-a * (a == 1));
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return ((int)num * a);
}
