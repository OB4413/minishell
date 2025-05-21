/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:47:20 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 16:48:33 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	help_handel_heredoc1(pid_t pid, int status, t_list_env *env, t_command **hh)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
		{
			(*hh)->heredoc = ft_strdup("ctrlC");
			env->value = ft_strdup("130");
			printf("\n");
			return (1);
		}
	}
	return (0);
}

int	help_handel_heredoc2(t_input *tok, t_command **hh, t_list_env *env, int fd)
{
	if ((tok->type == HEREDOC && !tok->next) || (tok->type == HEREDOC
			&& tok->next->type != WORD))
	{
		if (!tok->next)
		{
			(*hh)->heredoc = ft_strdup("ctrlC");
			env->value = ft_strdup("2");
			write(2, "minishell: syntax error near ", 30);
			write(2, "unexpected token `newline'\n", 28);
		}
		return (1);
	}
	if (tok->type == PIPE)
		*hh = (*hh)->next;
	if (fd > 0)
		close(fd);
	return (0);
}

int	help_handel_heredoc3(int *fd, t_command **hh, t_list_env *env, char *str)
{
	str = ft_strdup(str);
	if (*fd > 0)
		unlink((*hh)->heredoc);
	(*hh)->heredoc = random_str();
	*fd = open((*hh)->heredoc, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (*fd == -1)
	{
		env->value = ft_strdup("2");
		write(2, "minishell: i can not open the file\n", 36);
		return (1);
	}
	if (!check_qout_closed(str))
	{
		(*hh)->heredoc = ft_strdup("ctrlC");
		env->value = ft_strdup("2");
		return (1);
	}
	return (0);
}

int	help_run_children(char **tmp, t_list_env *env, char **str, t_input *tok)
{
	int	b;

	b = 0;
	*str = readline("> ");
	if ((*tmp)[0] != '"' && (*tmp)[0] != '\'' && str)
	{
		b = 2;
		if (ft_strcmp(*str, *tmp) != 0)
			expand_heredoc(str, env, 0);
	}
	*tmp = tok->next->value;
	remove_quote(tmp, 0);
	return (b);
}

void	run_children(char **tmp, t_list_env *env, t_input *tok, int fd)
{
	static int	j = 0;
	int			b;
	char		*str;

	j++;
	b = help_run_children(tmp, env, &str, tok);
	while (str && ft_strcmp(str, (*tmp)) != 0)
	{
		write(fd, str, strlen(str));
		write(fd, "\n", 1);
		j++;
		str = readline("> ");
		if (b == 2 && str)
		{
			if (ft_strcmp(str, (*tmp)) != 0)
				expand_heredoc(&str, env, 0);
		}
	}
	if (!str)
	{
		printf("minishell: warning: here-document at line ");
		printf("%d delimited by end-of-file (wanted `%s')\n", j, *tmp);
	}
	ft_malloc(0, 1);
	exit(0);
}
