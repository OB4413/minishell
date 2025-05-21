/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokns.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:34:00 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 16:52:02 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	handler_heredoc(t_input *tok, t_command **cmd_list, t_list_env *env,
		int fd)
{
	char		*tmp;
	pid_t		pid;
	t_command	*hh;

	help_handel_heredoc(tok, env, &hh, cmd_list);
	while (tok)
	{
		if (tok->next && tok->type == HEREDOC && tok->next->type == WORD)
		{
			if (help_handel_heredoc3(&fd, &hh, env, tok->next->value) == 1)
				return ;
			tmp = move_quote(ft_strdup(tok->next->value), 0, 0);
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, handler_sig_heredoc);
				run_children(&tmp, env, tok, fd);
			}
			if (help_handel_heredoc1(pid, 0, env, &hh) == 1)
				break ;
		}
		else if (help_handel_heredoc2(tok, &hh, env, fd) == 1)
			return ;
		tok = tok->next;
	}
}

int	help_chek_ambiguous_redirectin(t_list_env *env, t_redir **redir, int *i)
{
	(*redir)->filename = help_expand_variables((*redir)->filename, env, 0);
	(*redir)->filename = move_quote((*redir)->filename, 0, 0);
	if ((*redir)->filename && (*redir)->filename[0] != '\'')
	{
		while ((*redir)->filename[*i])
		{
			if ((*redir)->filename[*i] <= 32)
			{
				write(2, "minishell: ambiguous redirect\n", 31);
				return (1);
			}
			(*i)++;
		}
	}
	if (!(*redir)->filename)
	{
		write(2, "minishell: ambiguous redirect\n", 31);
		return (1);
	}
	remove_quote(&(*redir)->filename, 0);
	return (0);
}

int	check_ambiguous_redirect(t_command **cmd_list, t_list_env *env)
{
	t_command	*tmp;
	t_redir		*redir;
	int			i;

	i = 0;
	tmp = *cmd_list;
	while (tmp)
	{
		redir = tmp->inoutfile;
		while (redir)
		{
			if (redir->type == 2 || redir->type == 1)
			{
				if (help_chek_ambiguous_redirectin(env, &redir, &i) == 1)
					return (1);
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
	return (0);
}

void	max_heredoc(t_input *tmp)
{
	int	i;

	i = 0;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			i++;
		tmp = tmp->next;
	}
	if (i > 16)
	{
		write(2, "minishell: maximum here-document count exceeded\n", 49);
		ft_malloc(1, 1);
		rl_clear_history();
		exit(2);
	}
}

int	parsing_tokns(t_input *tok, t_command **cmd_list, t_list_env *env)
{
	t_input	*tmp;
	int		i;

	i = 0;
	max_heredoc(tok);
	tmp = tok;
	while (tmp)
	{
		if (tmp->type == PIPE && (!tmp->next || tmp->next->type == PIPE
				|| i == 0))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			env->value = ft_strdup("2");
			return (1);
		}
		tmp = tmp->next;
		i = 2;
	}
	handler_heredoc(tok, cmd_list, env, -2);
	if (check_ambiguous_redirect(cmd_list, env) == 1)
	{
		env->value = ft_strdup("1");
		return (1);
	}
	return (0);
}
