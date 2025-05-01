/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:35:50 by obarais           #+#    #+#             */
/*   Updated: 2024/11/01 11:21:22 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len_src;
	size_t	i;

	len_src = ft_strlen(src);
	i = 0;
	if (dstsize > 1)
	{
		while (i < dstsize - 1 && i < len_src)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	if (dstsize == 1)
		dst[0] = '\0';
	return (len_src);
}
