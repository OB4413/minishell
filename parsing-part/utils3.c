/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:42:12 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 16:43:42 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	help_move_quote(char **str, int *j, char **tmp)
{
	(*j)++;
	while ((*str)[*j] && (*str)[*j] != (*tmp)[0])
	{
		*tmp = ft_strjoin_c(*tmp, (*str)[*j]);
		(*j)++;
	}
	if ((*str)[*j] == (*tmp)[0])
	{
		(*j)++;
		while ((*str)[*j])
		{
			*tmp = ft_strjoin_c(*tmp, (*str)[*j]);
			(*j)++;
		}
		*tmp = ft_strjoin_c(*tmp, (*tmp)[0]);
	}
}

char	*check_qout_closed(char *str)
{
	int		i;
	char	q;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			q = str[i];
			i++;
			while (str[i] && str[i] != q)
				i++;
			if (!str[i])
			{
				write(2, "minishell: unexpected EOF while looking for", 44);
				write(2, " matching \"\'\n", 14);
				write(2, "minishell:syntax error:unexpected end of file\n", 47);
				return (NULL);
			}
		}
		i++;
	}
	return (str);
}

char	*move_quote(char *str, int i, int j)
{
	char	*tmp;

	tmp = NULL;
	while (str && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			tmp = ft_strjoin_c(tmp, str[i]);
			while (str[j] && str[j] != tmp[0])
			{
				tmp = ft_strjoin_c(tmp, str[j]);
				j++;
			}
			if (str[j] == tmp[0])
			{
				help_move_quote(&str, &j, &tmp);
				return (tmp);
			}
		}
		i++;
	}
	return (str);
}

void	remove_quote(char **str, int i)
{
	char	*tmp;
	char	qoute;

	tmp = NULL;
	while (*str && (*str)[i])
	{
		if ((*str)[i] == '"' || (*str)[i] == '\'')
		{
			qoute = (*str)[i];
			i++;
			while ((*str)[i] && (*str)[i] != qoute)
			{
				tmp = ft_strjoin_c(tmp, (*str)[i]);
				i++;
			}
			if ((*str)[i])
				i++;
		}
		else
		{
			tmp = ft_strjoin_c(tmp, (*str)[i]);
			i++;
		}
	}
	*str = tmp;
}

void	help_handel_heredoc(t_input *temp, t_list_env *env, t_command **hh,
		t_command **cmd_list)
{
	*hh = *cmd_list;
	while (temp)
	{
		if (temp->next && temp->type == HEREDOC && temp->next->type == WORD)
			temp = temp->next;
		else if (temp->next && temp->type == HEREDOC
			&& temp->next->type != WORD)
		{
			env->value = ft_strdup("2");
			write(2, "minishell: syntax error near unexpected token `<<'\n",
				52);
			break ;
		}
		else
			temp = temp->next;
	}
}
