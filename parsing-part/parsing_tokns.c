/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokns.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:34:00 by obarais           #+#    #+#             */
/*   Updated: 2025/05/17 09:32:47 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*random_str(void)
{
	static int	count = 0;
	char		*str;

	str = ft_strjoin("/tmp/heredoc+", ft_itoa(count));
	count++;
	return (str);
}

void	expand_heredoc(char **str, list_env *env)
{
	char	*tmp;
	int		i;
	int		start;

	i = 0;
	start = 0;
	tmp = NULL;
	while ((*str)[i])
	{
		if ((*str)[i] == '$' && (*str)[i + 1] && (ft_isalnum((*str)[i + 1])
				|| (*str)[i + 1] == '_'))
		{
			i++;
			if ((*str)[i] >= 48 && (*str)[i] <= 57)
			{
				i++;
				continue ;
			}
			start = i;
			while ((*str)[i] && (ft_isalnum((*str)[i]) || (*str)[i] == '_'))
				i++;
			tmp = ft_strjoin(tmp, get_value(ft_substr((*str), start, i - start),
						env));
		}
		else
		{
			tmp = ft_strjoin_c(tmp, (*str)[i]);
			i++;
		}
	}
	*str = "\0";
	if (tmp)
		*str = tmp;
}

char	*move_quote(char *str)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
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
				j++;
				while (str[j] && str[j] != tmp[0])
				{
					tmp = ft_strjoin_c(tmp, str[j]);
					j++;
				}
				if (str[j] == tmp[0])
				{
					j++;
					while (str[j])
					{
						tmp = ft_strjoin_c(tmp, str[j]);
						j++;
					}
					tmp = ft_strjoin_c(tmp, tmp[0]);
				}
				return (tmp);
			}
		}
		i++;
	}
	return (str);
}

void	remove_quote(char **str)
{
	char	*tmp;
	int		i;
	char	qoute;

	i = 0;
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

void	handler_heredoc(t_input *tok, t_command **cmd_list, list_env *env)
{
	int			fd;
	char		*str;
	char		*tmp;
	t_input		*temp;
	static int	j = 0;
	int			status;
	int			b;
	pid_t		pid;
	int			sig;

	b = 0;
	fd = -2;
	temp = tok;
	while (temp)
	{
		if (temp->next && temp->type == HEREDOC && temp->next->type == WORD)
			temp = temp->next;
		else if (temp->next && temp->type == HEREDOC
			&& temp->next->type != WORD)
		{
			printf("%s\n",
				"minishell: syntax error near unexpected token `<<'");
			break ;
		}
		else
			temp = temp->next;
	}
	while (tok)
	{
		if (tok->next && tok->type == HEREDOC && tok->next->type == WORD)
		{
			if (fd != -2)
				unlink(tmp);
			(*cmd_list)->heredoc = random_str();
			fd = open((*cmd_list)->heredoc, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (fd == -1)
			{
				write(2, "minishell: i can not open the file\n", 36);
				return ;
			}
			tmp = move_quote(ft_strdup(tok->next->value));
			if (tmp[0] == '"' || tmp[0] == '\'')
			{
				if (tmp[ft_strlen(tmp) - 1] != tmp[0])
				{
					(*cmd_list)->heredoc = ft_strdup("ctrlC");
					write(2, "minishell: unexpected EOF while looking for", 44);
					write(2, " matching \"\'\n", 14);
					write(2, "minishell:syntax error:unexpected end of file\n",
						49);
					break ;
				}
			}
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				j++;
				str = readline("> ");
				if (tmp[0] != '"' && tmp[0] != '\'' && str)
				{
					b = 2;
					expand_heredoc(&str, env);
				}
				tmp = tok->next->value;
				remove_quote(&tmp);
				while (str && ft_strcmp(str, tmp) != 0)
				{
					write(fd, str, strlen(str));
					write(fd, "\n", 1);
					j++;
					str = readline("> ");
					if (b == 2 && str)
						expand_heredoc(&str, env);
				}
				if (!str)
				{
					printf("minishell: warning: here-document at line");
					printf("%d delimited by end-of-file (wanted `%s')\n", j,
						tmp);
				}
				exit(0);
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				waitpid(pid, &status, 0);
				signal(SIGINT, sigint_handler);
				if (WIFSIGNALED(status))
				{
					sig = WTERMSIG(status);
					if (sig == SIGINT)
					{
						(*cmd_list)->heredoc = ft_strdup("ctrlC");
						printf("\n");
						break ;
					}
				}
			}
		}
		else if ((tok->type == HEREDOC && !tok->next) || (tok->type == HEREDOC
				&& tok->next->type != WORD))
		{
			if (!tok->next)
			{
				write(2, "minishell: syntax error near ", 30);
				write(2, "unexpected token `newline'\n", 28);
			}
			return ;
		}
		tok = tok->next;
		close(fd);
	}
}

void	chek_ambiguous_redirect(t_command **cmd_list, list_env *env)
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
				redir->filename = help_expand_variables(redir->filename, env);
				redir->filename = move_quote(redir->filename);
				if (redir->filename && redir->filename[0] != '\'')
				{
					while (redir->filename[i])
					{
						if (redir->filename[i] <= 32)
						{
							printf("minishell: ambiguous redirect\n");
							break ;
						}
						i++;
					}
				}
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
}

int	parsing_tokns(t_input *tok, t_command **cmd_list, list_env *env)
{
	t_input	*tmp;
	int		i;

	i = 0;
	tmp = tok;
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
	tmp = tok;
	if (tmp->type == PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		env->value = ft_strdup("2");
		return (1);
	}
	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->type == PIPE && (!tmp->next || tmp->next->type == PIPE))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			env->value = ft_strdup("2");
			return (1);
		}
		tmp = tmp->next;
	}
	handler_heredoc(tok, cmd_list, env);
	chek_ambiguous_redirect(cmd_list, env);
	return (0);
}
