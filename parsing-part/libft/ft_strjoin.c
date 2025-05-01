/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 11:31:49 by obarais           #+#    #+#             */
/*   Updated: 2024/11/07 12:50:41 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*p;
	size_t		j;
	size_t		k;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup(s2));
	if (s2 == NULL)
		return (ft_strdup(s1));
	p = ft_calloc((ft_strlen(s1) + ft_strlen(s2) + 1), sizeof(char));
	j = 0;
	k = 0;
	if (p == NULL)
		return (NULL);
	while (s1[j] != '\0')
	{
		p[j] = s1[j];
		j++;
	}
	while (s2[k] != '\0')
		p[j++] = s2[k++];
	p[j] = '\0';
	return (p);
}
