/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:40:55 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 17:13:54 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_word(char const *p, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (p[i] == c && p[i] != '\0')
		i++;
	while (p[i] != '\0')
	{
		if (p[i] != c && (i == 0 || p[i - 1] == c))
			j++;
		i++;
	}
	return (j);
}

static char	*ft_alloc_and_cpy(char const *s, char c, size_t *i)
{
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	while (s[*i] != '\0' && s[*i] != c)
	{
		len++;
		(*i)++;
	}
	return (ft_substr(s, start, len));
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	words;
	char	**array;

	if (s == NULL)
		return (NULL);
	i = 0;
	j = 0;
	words = ft_count_word(s, c);
	array = (char **)malloc((words + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	while (j < words)
	{
		while (s[i] == c)
			i++;
		array[j] = ft_alloc_and_cpy(s, c, &i);
		if (!array[j])
			return (NULL);
		j++;
	}
	array[j] = NULL;
	return (array);
}
