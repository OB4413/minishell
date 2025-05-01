/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:12:53 by obarais           #+#    #+#             */
/*   Updated: 2024/11/06 08:44:41 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	copy_ward(unsigned char *dst, const unsigned char *src, size_t len)
{
	while (len > 0)
	{
		len--;
		dst[len] = src[len];
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*p;
	const unsigned char	*b;
	size_t				i;

	p = (unsigned char *)dst;
	b = (const unsigned char *)src;
	i = 0;
	if (p == b || len == 0)
		return (dst);
	if (p < b)
	{
		while (i < len)
		{
			p[i] = b[i];
			i++;
		}
	}
	if (p > b)
		copy_ward(p, b, len);
	return (dst);
}
