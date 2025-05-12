/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:29:24 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/09 10:39:34 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static void	ftstrlcpy(const char *src, char *dst, int c)
{
	int	i;

	i = 0;
	while (i < c - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

static int	safe_malloc(char **s, int k, int c)
{
	s[k] = (char *)ft_malloc((c * sizeof(char) + 1), 0);
	if (s[k] == NULL)
	{
		while (k >= 0)
		{
			k--;
		}
		return (0);
	}
	return (1);
}

static int	count_words(const char *s, char c)
{
	int	i;
	int	is_c;
	int	words;

	i = 0;
	is_c = 0;
	words = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && is_c == 0)
		{
			words++;
			is_c = 1;
		}
		if (is_c == 1 && s[i] == c)
			is_c = 0;
		i++;
	}
	return (words);
}

static int	fill(char **s1, const char *s, char c)
{
	int	i;
	int	len;
	int	k;

	k = 0;
	i = 0;
	while (s[i] != '\0')
	{
		len = 0;
		while (s[i] == c)
			i++;
		while (s[i] != '\0' && s[i] != c)
		{
			i++;
			len++;
		}
		if (s[i] != '\0' || (s[i] == '\0' && s[i - 1] != c))
		{
			if (safe_malloc(s1, k, len) == 0)
				return (1);
			ftstrlcpy(&s[i - len], s1[k], len + 1);
			k++;
		}
	}
	return (0);
}

char	**ft_split1(char const *s, char c)
{
	int		words;
	char	**s1;

	if (s == NULL)
		return (NULL);
	words = count_words(s, c);
	s1 = (char **)ft_malloc(((words + 1) * sizeof(char *)), 0);
	if (s1 == NULL)
		return (NULL);
	if (fill(s1, s, c) == 1)
	{
		return (0);
	}
	s1[words] = NULL;
	return (s1);
}
