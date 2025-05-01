/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:12:54 by obarais           #+#    #+#             */
/*   Updated: 2024/11/06 11:07:31 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p;
	const unsigned char	*b;
	size_t				i;

	p = (const unsigned char *)s1;
	b = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p[i] == b[i])
			i++;
		else
			return (p[i] - b[i]);
	}
	return (0);
}
