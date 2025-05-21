/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:50:58 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 17:14:17 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	handler_sig_heredoc(int sig)
{
	(void)sig;
	ft_malloc(0, 1);
	exit(130);
}

void	help_count_word(char *line, int *k, int *i, char *d)
{
	(*i)++;
	if (line[*i] && line[*i] == *d)
	{
		*d = '\0';
		*k = 0;
		(*i)++;
	}
}

void	skip_wait_space(char *line, int *i)
{
	*i = 0;
	while (line[*i] <= 32 && line[*i] != '\0')
		(*i)++;
}

int	ft_count_word_min(char *line, int j, int k, char d)
{
	int	i;

	skip_wait_space(line, &i);
	while (line[i] != '\0')
	{
		if ((line[i] == '"' || line[i] == '\'') && k == 0)
		{
			if ((i == 0 || line[i - 1] <= 32 || line[i - 1] == '|'
					|| ft_strrchr("><", line[i - 1])))
				j++;
			d = line[i];
			k = 1;
		}
		if ((line[i] == '|' || ft_strchr("><", line[i])) && k == 0)
		{
			if (line[i] == line[i + 1])
				i++;
			j++;
		}
		else if (line[i] > 32 && k == 0 && (i == 0 || line[i - 1] <= 32
				|| line[i - 1] == '|' || ft_strrchr("><", line[i - 1])))
			j++;
		help_count_word(line, &k, &i, &d);
	}
	return (j);
}

char	*get_value(char *str, t_list_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, str) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
