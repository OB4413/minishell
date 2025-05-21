/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/21 15:35:23 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	is_redirection(t_redir *redir, t_list_env **env_list, char *file)
{
	t_redir	*redirection;

	redirection = redir;
	while (redirection)
	{
		if (redirection->type == 1 || redirection->type == 2)
		{
			if (handle_output_redirection(redirection, env_list) < 0)
				return (-1);
		}
		else if (redirection->type == 0)
		{
			if (handle_input_redirection(redirection, env_list) < 0)
				return (-1);
		}
		else if (redirection->type == 3)
		{
			if (heredoc_redirection(redir, file, env_list) == -1)
				return (-1);
			else if (heredoc_redirection(redir, file, env_list) == -2)
				return (0);
		}
		redirection = redirection->next;
	}
	return (0);
}

int	heredoc_redirection(t_redir *redir, char *file, t_list_env **env_list)
{
	int	fd;

	if (!file || ft_strcmp(file, "ctrlC") == 0)
		return (-1);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (perror("heredoc open"), (*env_list)->value = ft_strdup("1"),
			-1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), (*env_list)->value = ft_strdup("1"), close(fd),
			-1);
	close(fd);
	while (redir && redir->type == 3)
		redir = redir->next;
	if (!redir)
		return (-2);
	return (0);
}

int	ft_lstsize1(t_command *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	execute_single(t_command *cmd, t_list_env **env_list)
{
	char	*file;

	file = cmd->heredoc;
	if (before_redir(cmd, env_list, file) != 1)
		execute_cmd(cmd, env_list);
}

void	setup_pipe(t_command *cmd, int pipe_fd[2])
{
	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		ft_exit_status(1);
	}
}
