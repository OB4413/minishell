/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:29:19 by obarais           #+#    #+#             */
/*   Updated: 2024/10/27 10:28:56 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p;
	unsigned char		a;
	size_t				i;

	p = (const unsigned char *)s;
	a = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (p[i] == a)
			return ((void *)&p[i]);
		i++;
	}
	return (NULL);
}
