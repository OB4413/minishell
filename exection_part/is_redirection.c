/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/09 19:22:07 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int get_flags(t_command *cmd2)
{
	int	flags;

	if (cmd2->inoutfile->type == 1)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	return (flags);
}

int	is_redirection(t_command *cmd, list_env **env_list, char ***env,
		char ***env1)
{
	t_command	*cmd2;

	int (fd), (flags), (saved_stdout);
	saved_stdout = dup(1);
	cmd2 = cmd;
	if (cmd2->inoutfile && (cmd2->inoutfile->type == 1
			|| cmd2->inoutfile->type == 2))
	{
		flags = get_flags(cmd2);
		fd = open(cmd2->inoutfile->filename, flags, 0644);
		if (fd == -1)
			return (perror("fd error :"), 1);
		if (dup2(fd, 1) == -1)
			return (perror("dup2 failed"), 0);
		close(fd);
		execute_cmd(cmd, env_list, env, env1);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		return (1);
	}
	else if (cmd2->inoutfile && (cmd2->inoutfile->type == 0
			|| cmd2->inoutfile->type == 3))
		return (in_heredoc_redirs(cmd, env_list, env, env1), 1);
	else
		return (0);
}

void	heredoc_redirection(struct s_command *cmd, list_env **env_list,
		char ***env, char ***env1)
{
	int (fd), (saved_stdout);
	saved_stdout = dup(0);
	fd = open(cmd->heredoc, O_RDONLY);
	if (fd == -1)
		return (perror("open REDIRECT_IN"));
	dup2(fd, 0);
	execute_cmd(cmd, env_list, env, env1);
	close(fd);
	dup2(saved_stdout, 0);
	close(saved_stdout);
	return ;
}

void	in_heredoc_redirs(struct s_command *cmd, list_env **env_list,
		char ***env, char ***env1)
{
	int (fd), (saved_stdout);
	saved_stdout = dup(0);
	if (cmd->inoutfile->type == 0)
	{
		fd = open(cmd->inoutfile->filename, O_RDONLY);
		if (fd == -1)
			return (perror("open REDIRECT_IN"));
		dup2(fd, 0);
		execute_cmd(cmd, env_list, env, env1);
		close(fd);
		dup2(saved_stdout, 0);
		close(saved_stdout);
		return ;
	}
	else
		heredoc_redirection(cmd, env_list, env, env1);
}

void	execute_piped_commands(t_command *cmd_list, list_env **env_list,
		char ***env, char ***env1)
{
	pid_t	pid;

	int (pipe_fd[2]), (prev_fd);
	prev_fd = -1;
	if (!cmd_list->next)
	{
		if (is_redirection(cmd_list, env_list, env, env1) != 1)
			execute_cmd(cmd_list, env_list, env, env1);
		return ;
	}
	while (cmd_list)
	{
		if (cmd_list->next)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"));
		}
		pid = fork();
		if (pid == -1)
			return (perror("fork"));
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if (cmd_list->next)
			{
				dup2(pipe_fd[1], 1);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (is_redirection(cmd_list, env_list, env, env1) != 1)
				execute_cmd(cmd_list, env_list, env, env1);
			exit(1);
		}
		else
		{
			waitpid(pid, NULL, 0);
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd_list->next)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			cmd_list = cmd_list->next;
		}
	}
}
