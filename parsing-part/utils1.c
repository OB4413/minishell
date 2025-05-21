/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:27:49 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 17:06:21 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	help_list_env1(t_list_env **new_env, int *c, t_list_env **env_list)
{
	if (ft_strcmp((*new_env)->key, "SHLVL") == 0)
	{
		*c = ft_atoi((*new_env)->value);
		(*c)++;
		if (*c > 999)
		{
			printf("warning: shell level (%d) too high, resetting to 1\n", *c);
			*c = 1;
		}
		else if (*c < 0)
			*c = 0;
		(*new_env)->value = ft_strdup(ft_itoa(*c));
	}
	(*new_env)->equal = 1;
	(*new_env)->next = NULL;
	if (*env_list == NULL)
		*env_list = *new_env;
}

void	help_list_env(char **env, t_list_env **new_env, t_list_env **tmp,
		t_list_env **env_list)
{
	int (i), (j), (c);
	i = 0;
	j = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != '=')
			j++;
		*new_env = ft_malloc(sizeof(t_list_env), 0);
		if (!(*new_env))
			return ;
		(*new_env)->key = ft_substr(env[i], 0, j);
		(*new_env)->value = ft_substr(env[i], j + 1, ft_strlen(env[i]) - j);
		help_list_env1(new_env, &c, env_list);
		if (*env_list)
		{
			*tmp = *env_list;
			while ((*tmp)->next != NULL)
				*tmp = (*tmp)->next;
			(*tmp)->next = *new_env;
		}
		i++;
	}
}

void	ft_list_env(char **env, t_list_env **env_list)
{
	t_list_env	*new_env;
	t_list_env	*tmp;

	new_env = ft_malloc(sizeof(t_list_env), 0);
	new_env->key = ft_strdup("$?");
	new_env->value = "0";
	new_env->next = NULL;
	*env_list = new_env;
	help_list_env(env, &new_env, &tmp, env_list);
}

char	**help_put_the_args(char **args, t_input **tmp, int *i, int *j)
{
	args = ft_malloc(sizeof(char *) * (*i + 1), 0);
	if (args == NULL)
		return (NULL);
	while (*tmp && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == HEREDOC || (*tmp)->type == APPEND
			|| (*tmp)->type == REDIRECT_IN || (*tmp)->type == REDIRECT_OUT)
		{
			if (!(*tmp)->next)
				break ;
			*tmp = (*tmp)->next->next;
			continue ;
		}
		if ((*tmp)->value)
		{
			args[*j] = ft_strdup((*tmp)->value);
			(*j)++;
		}
		*tmp = (*tmp)->next;
	}
	args[*j] = NULL;
	return (args);
}

char	**put_the_args(t_input *tok, char *cmd, char **args, t_input *tmp)
{
	int (i), (j);
	t_input(*tmp2);
	i = 0;
	j = 0;
	tmp = tok;
	while (ft_strcmp(tmp->value, cmd) != 0)
		tmp = tmp->next;
	tmp2 = tmp;
	while (tmp2 && tmp2->type != PIPE)
	{
		if (tmp2->type == HEREDOC || tmp2->type == APPEND
			|| tmp2->type == REDIRECT_IN || tmp2->type == REDIRECT_OUT)
		{
			if (!tmp2->next)
				break ;
			tmp2 = tmp2->next->next;
			continue ;
		}
		if (tmp2->value)
			i++;
		tmp2 = tmp2->next;
	}
	return (help_put_the_args(args, &tmp, &i, &j));
}
