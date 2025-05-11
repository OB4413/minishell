/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokns.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:34:00 by obarais           #+#    #+#             */
/*   Updated: 2025/05/11 21:44:12 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int c_or_d = 0;

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
	*str = "\0";
	if (tmp)
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
	while (*str && (*str)[i])
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
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	close(0);
	c_or_d = 1;
}

void	signl_1(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handler_heredoc(t_input *tok, t_command **cmd_list, list_env *env)
{
	int fd;
	char *str;
	char *tmp;
	t_input *temp;
	static int j = 0;
	int b = 0;

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
			signal(SIGINT, handl_2);
			j++;
			str = readline("> ");
			if (tmp[0] != '"' && tmp[0] != '\'' && str)
			{
				b = 2;
				expand_heredoc(&str, env);
			}
			else if (tmp[0] == '"' || tmp[0] == '\'')
			{
				if (tmp[ft_strlen(tmp)- 1] != tmp[0])
				{
					b = -100;
					tmp = NULL;
				}
			}
			remove_quote(&tmp);
			while (str && ft_strcmp(str, tmp) != 0)
			{
				write(fd, str, strlen(str));
				write(fd, "\n", 1);
				j++;
				readline("> ");
				if (b && str)
					expand_heredoc(&str, env);
			}
			if (!str && c_or_d == 0)
			{
				if (b == -100)
				{
					(*cmd_list)->heredoc = ft_strdup("ctrlC");
					printf("\nminishell: unexpected EOF while looking for matching \"\'\n");
					printf("minishell: syntax error: unexpected end of file\n");
				}
				else
					printf("\nminishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", j, tmp);
			}
			open("/dev/tty", O_RDONLY);
			if (!str && c_or_d == 1)
			{
				(*cmd_list)->heredoc = ft_strdup("ctrlC");
				c_or_d = 0;
				break;
			}
		}
		else if ((tok->type == HEREDOC && !tok->next) || (tok->type == HEREDOC && tok->next->type != WORD))
		{
			if (!tok->next)
			printf("\nminishell: syntax error near unexpected token `newline'\n");
			return ;
		}
		tok = tok->next;
	}
	signal(SIGINT, signl_1);
	close(fd);
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
			if (redir->type == 2 || redir->type == 1)
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
