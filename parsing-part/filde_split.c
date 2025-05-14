/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filde_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:14:33 by obarais           #+#    #+#             */
/*   Updated: 2025/05/14 18:12:59 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

static int	ft_count_word_7(char const *p)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (p[i] <= 32 && p[i] != '\0')
		i++;
	while (p[i] != '\0')
	{
		if (p[i] > 32 && (i == 0 || p[i - 1] <= 32))
			j++;
		i++;
	}
	return (j);
}

static char	*ft_alloc_and_cpy_7(char const *s, size_t *i)
{
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	while (s[*i] != '\0' && s[*i] > 32)
	{
		len++;
		(*i)++;
	}
	return (ft_substr(s, start, len));
}

char	**ft_split_7(char const *s)
{
	size_t	i;
	size_t	j;
	size_t	words;
	char	**array;

	if (s == NULL)
		return (NULL);
	i = 0;
	j = 0;
	words = ft_count_word_7(s);
	array = (char **)malloc((words + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	while (j < words)
	{
		while (s[i] <= 32)
			i++;
		array[j] = ft_alloc_and_cpy_7(s, &i);
		if (!array[j])
			return (NULL);
		j++;
	}
	array[j] = NULL;
	return (array);
}
