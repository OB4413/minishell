/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable_Expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 08:06:45 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 17:05:54 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	help_expand_variables2(int *i, char *str, char **tokn, t_list_env *env)
{
	char	q;
	int		start;

	q = str[*i];
	*tokn = ft_strjoin_c(*tokn, '\'');
	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != q)
		(*i)++;
	if (str[*i])
		(*i)++;
	*tokn = ft_strjoin(*tokn, ft_check_quote(ft_substr(str, start, *i - start),
				env, q, 0));
	*tokn = ft_strjoin_c(*tokn, '\'');
}

char	*help_expand_variables(char *str, t_list_env *env, int i)
{
	char	*tokn;

	tokn = NULL;
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
					+ 1] == '_'))
		{
			if (help_expand_variables1(&i, str, env, &tokn) == 1)
				continue ;
		}
		else if (str[i] == '"' || str[i] == '\'')
			help_expand_variables2(&i, str, &tokn, env);
		else
		{
			tokn = ft_strjoin_c(tokn, str[i]);
			i++;
		}
	}
	return (tokn);
}

void	heelp_expand_variables1(t_input **temp, int *i, char **tokn,
		t_list_env *env)
{
	int		start;
	char	q;

	if ((*temp)->value[*i] == '"' || (*temp)->value[*i] == '\'')
	{
		q = (*temp)->value[*i];
		start = *i;
		(*i)++;
		while ((*temp)->value[*i] && (*temp)->value[*i] != q)
			(*i)++;
		if ((*temp)->value[*i])
			(*i)++;
		*tokn = ft_strjoin(*tokn, ft_check_quote(ft_substr((*temp)->value,
						start, *i - start), env, q, 0));
	}
	else
	{
		*tokn = ft_strjoin_c(*tokn, (*temp)->value[*i]);
		(*i)++;
	}
}

void	heelp_expand_variables(t_input **temp, int *i, char **tokn,
		t_list_env *env)
{
	int (start);
	if ((*temp)->value[*i] == '$' && (*temp)->value[*i + 1]
		&& (ft_isalnum((*temp)->value[*i + 1]) || (*temp)->value[*i + 1] == '_'
			|| (*temp)->value[*i + 1] == '?'))
	{
		if ((*i)++ && (*temp)->value[*i] >= 48 && (*temp)->value[*i] <= 57
			&& (*i)++)
			return ;
		start = *i;
		while ((*temp)->value[*i] && (ft_isalnum((*temp)->value[*i])
				|| (*temp)->value[*i] == '_' || (*temp)->value[*i] == '?'))
		{
			if ((*temp)->value[++(*i) - 1] == '?' && start--)
				break ;
		}
		if ((*temp)->value[*i - 1] == '?')
			*tokn = ft_strjoin(*tokn, get_value(ft_substr((*temp)->value, start,
							*i - start), env));
		else
			*tokn = split_to_tokens(*tokn, temp,
					get_value(ft_substr((*temp)->value, start, *i - start),
						env), (*temp)->value[*i]);
	}
	else
		heelp_expand_variables1(temp, i, tokn, env);
}

void	expand_variables(t_input **tok, t_list_env *env, int i)
{
	char	*tokn;
	t_input	*temp;

	temp = *tok;
	tokn = NULL;
	while (temp)
	{
		i = 0;
		if (temp->value && temp->type == WORD)
		{
			while (temp->value[i])
				heelp_expand_variables(&temp, &i, &tokn, env);
			if (tokn == NULL && ft_strchr(temp->value, '$') == NULL)
				tokn = ft_strdup("\0");
			temp->value = tokn;
			tokn = NULL;
		}
		else if ((temp->type == HEREDOC || temp->type == REDIRECT_OUT
				|| temp->type == APPEND) && temp->next)
			temp = temp->next;
		temp = temp->next;
	}
}
