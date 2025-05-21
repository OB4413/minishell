/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:35:33 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 16:44:03 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	what_direction(char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (0);
	else if (ft_strcmp(str, ">") == 0)
		return (1);
	else if (ft_strcmp(str, ">>") == 0)
		return (2);
	else if (ft_strcmp(str, "<<") == 0)
		return (3);
	return (-1);
}

void	help_check_derction(t_input **tmp, t_redir **new_redir, t_redir **tmp2,
		t_redir **redir)
{
	while (*tmp && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == HEREDOC || (*tmp)->type == APPEND
			|| (*tmp)->type == REDIRECT_IN || (*tmp)->type == REDIRECT_OUT)
		{
			*new_redir = ft_malloc(sizeof(t_redir), 0);
			if ((*tmp)->next)
				(*new_redir)->filename = ft_strdup((*tmp)->next->value);
			else
				(*new_redir)->filename = NULL;
			(*new_redir)->type = what_direction((*tmp)->value);
			(*new_redir)->next = NULL;
			if (*redir == NULL)
				*redir = *new_redir;
			else
			{
				*tmp2 = *redir;
				while ((*tmp2)->next != NULL)
					*tmp2 = (*tmp2)->next;
				(*tmp2)->next = *new_redir;
			}
		}
		*tmp = (*tmp)->next;
	}
}

char	*random_str(void)
{
	static int	count = 0;
	char		*str;

	str = ft_strjoin("/tmp/heredoc+", ft_itoa(count));
	count++;
	return (str);
}

int	help_expand_heredoc(char **str, int *i, int *start)
{
	(*i)++;
	if ((*str)[*i] >= 48 && (*str)[*i] <= 57)
	{
		(*i)++;
		return (1);
	}
	*start = *i;
	while ((*str)[*i] && (ft_isalnum((*str)[*i]) || (*str)[*i] == '_'
			|| (*str)[*i] == '?'))
	{
		(*i)++;
		if ((*str)[*i - 1] == '?' && (*start)--)
			break ;
	}
	return (0);
}

void	expand_heredoc(char **str, t_list_env *env, int i)
{
	char	*tmp;
	int		start;

	start = 0;
	tmp = NULL;
	while ((*str)[i])
	{
		if ((*str)[i] == '$' && (*str)[i + 1] && (ft_isalnum((*str)[i + 1])
				|| (*str)[i + 1] == '_' || (*str)[i + 1] == '?'))
		{
			if (help_expand_heredoc(str, &i, &start) == 1)
				continue ;
			if ((*str)[i - 1] == '?')
				tmp = ft_strjoin(tmp, get_value(ft_substr((*str), start, i
								- start), env));
			else
				tmp = ft_strjoin(tmp, get_value(ft_substr((*str), start, i
								- start), env));
		}
		else
			tmp = ft_strjoin_c(tmp, (*str)[i++]);
	}
	*str = "\0";
	if (tmp)
		*str = tmp;
}
