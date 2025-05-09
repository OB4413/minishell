/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokns.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:34:00 by obarais           #+#    #+#             */
/*   Updated: 2025/05/09 15:25:33 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char *random_str()
{
	static int count = 0;
	char *str;

	str = ft_strjoin("/tmp/heredoc+", ft_itoa(count));
	count++;
	return (str);
}

void	expand_heredoc(char **str, list_env *env)
{
	char *tmp;
	int i = 0;
	int start = 0;

	tmp = NULL;
	while ((*str)[i])
	{
		if ((*str)[i] == '$' && (*str)[i + 1] && (ft_isalnum((*str)[i + 1]) || (*str)[i + 1] == '_'))
		{
			i++;
			if ((*str)[i] >= 48 && (*str)[i] <= 57)
			{
				i++;
				continue;
			}
			start = i;
			while ((*str)[i] && (ft_isalnum((*str)[i]) || (*str)[i] == '_'))
				i++;
			tmp = ft_strjoin(tmp, get_value(ft_substr((*str), start, i - start), env));
		}
		else
		{
			tmp = ft_strjoin_c(tmp, (*str)[i]);
			i++;
		}
	}
	*str = tmp;
}
char *move_quote(char *str)
{
	char *tmp;
	int i = 0;
	int j = 0;

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
				else
				{
					printf("minishell: syntax error near unexpected token `%s'\n", str);
					exit(1);
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
	char *tmp;
	int i = 0;
	char qoute;

	tmp = NULL;
	while ((*str)[i])
	{
		if((*str)[i] == '"' || (*str)[i] == '\'')
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

void	handl_2(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		close(0);
	}
}

void	handler_heredoc(t_input *tok, t_command **cmd_list, list_env *env)
{
	int fd;
	char *str;
	char *tmp;
	t_input *temp;
	int saved_stdin;

	fd = -2;
	temp = tok;
	while (temp)
	{
		if (temp->next && temp->type == HEREDOC && temp->next->type == WORD)
			temp = temp->next;
		else if (temp->next && temp->type == HEREDOC && temp->next->type != WORD)
		{
			printf("%s\n", "minishell: syntax error near unexpected token `<<'");
			break;
		}
		else
			temp = temp->next;
	}
	while (tok)
	{
		if (tok->next && tok->type == HEREDOC && tok->next->type == WORD && fd != -2)
			unlink(tmp);
		if (tok->next && tok->type == HEREDOC && tok->next->type == WORD)
		{
			(*cmd_list)->heredoc = random_str();
			fd = open((*cmd_list)->heredoc, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (fd == -1)
			{
				printf("minishell: i can not open the file\n");
				exit(1);
			}
			tmp = move_quote(tok->next->value);
			saved_stdin = dup(STDIN_FILENO);
			signal(SIGINT, handl_2);
			str = readline("> ");
			if (tmp[0] != '"' && tmp[0] != '\'' && str)
					expand_heredoc(&str, env);
			remove_quote(&tmp);
			while (str && ft_strcmp(str, tmp) != 0)
			{
				write(fd, str, strlen(str));
				write(fd, "\n", 1);
				str = readline("> ");
			}
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
			signal(SIGINT, sigint_handler);
		}
		else if ((tok->type == HEREDOC && !tok->next) || (tok->type == HEREDOC && tok->next->type != WORD))
		{
			if (!tok->next)
				printf("minishell: syntax error near unexpected token `newline'\n");
			return ;
		}
		tok = tok->next;
	}
}

void	chek_ambiguous_redirect(t_command **cmd_list, list_env *env)
{
	t_command *tmp;
	t_redir *redir;
	int i = 0;

	tmp = *cmd_list;
	while (tmp)
	{
		redir = tmp->inoutfile;
		while (redir)
		{
			if (redir->type == 2 || redir->type == 0)
			{
				redir->filename = help_expand_variables(redir->filename, env);
				redir->filename = move_quote(redir->filename);
				if (redir->filename && redir->filename[0] != '\'')
				{
					while(redir->filename[i])
					{
						if (redir->filename[i] <= 32)
						{
							printf("minishell: ambiguous redirect\n");
							exit(1);
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

void 	parsing_tokns(t_input *tok, t_command **cmd_list, list_env *env)
{
	t_input *tmp;
	int i = 0;

	tmp = tok;
	while (tmp)
	{
		if(tmp->type == HEREDOC)
			i++;
		tmp = tmp->next;
	}
	if (i > 16)
	{
		printf("minishell: maximum here-document count exceeded\n");
		exit(1);
	}
	handler_heredoc(tok, cmd_list, env);
	chek_ambiguous_redirect(cmd_list, env);
}
