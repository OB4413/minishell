/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:54:39 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 16:57:07 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*ft_strjoin_c(char *s1, char c)
{
	char	*str;
	int		i;

	if (!s1)
	{
		str = ft_malloc(2, 0);
		str[0] = c;
		str[1] = '\0';
		return (str);
	}
	i = 0;
	while (s1[i])
		i++;
	str = ft_malloc(i + 2, 0);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = c;
	str[i + 1] = '\0';
	return (str);
}

char	*help_split_to_tokens(char h, t_input **temp, t_input **temp1,
		char **temp2)
{
	t_input	*new;
	char	*k;

	if (h)
	{
		new = ft_malloc(sizeof(t_input), 0);
		new->value = *temp2;
		new->type = WORD;
		(*temp)->next = new;
		*temp = (*temp)->next;
		(*temp)->next = *temp1;
		return (NULL);
	}
	k = (*temp)->value;
	(*temp)->value = *temp2;
	(*temp)->next = *temp1;
	return (k);
}

int	help_split_to_tokens1(char *str, t_input **temp, int *i, char *tokn)
{
	int	start;

	start = *i;
	while (str[*i] && str[*i] > 32)
		(*i)++;
	if (!(*temp)->next)
	{
		if (!tokn)
			(*temp)->value = ft_substr(str, start, *i - start);
		else
			(*temp)->value = ft_substr(str, 0, *i);
		(*temp)->next = *temp;
		(*i)++;
		return (1);
	}
	return (0);
}

char	*help_split_to_tokens2(char *k, t_input **temp, t_input **temp1,
		char **temp2)
{
	k = (*temp)->value;
	(*temp)->value = *temp2;
	(*temp)->next = *temp1;
	return (k);
}

void	help_split_to_tokens3(char *str, t_input **temp, int *i, int *start)
{
	t_input	*new;

	new = ft_malloc(sizeof(t_input), 0);
	new->value = ft_substr(str, *start, *i - *start);
	new->type = WORD;
	new->next = new;
	(*temp)->next = new;
	(*temp) = (*temp)->next;
}
