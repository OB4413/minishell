/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 10:13:35 by obarais           #+#    #+#             */
/*   Updated: 2024/11/07 10:24:23 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*p;

	i = ft_strlen(s1);
	p = ft_calloc((i + 1), sizeof(char));
	if (p == NULL)
		return (NULL);
	ft_memcpy(p, s1, i);
	p[i] = '\0';
	return (p);
}
