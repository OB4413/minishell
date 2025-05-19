/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/19 19:07:19 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	get_flags(t_command *cmd2)
{
	int	flags;

	if (cmd2->inoutfile->type == 1)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	return (flags);
}

int	multiple_out(t_command **cmd2, int flags, t_list_env **env_list, char ***env,
	char ***env1, char *file2)
{
	t_command	*cmd;
	int			fd;

	(void)env;
	(void)env1;
	(void)file2;
	t_redir		(*file), (*last);
	cmd = *cmd2;
	file = cmd->inoutfile;
	last = NULL;
	while (file)
	{
		if (file->type == 1 || file->type == 2)
		{
			fd = open(file->filename, flags, 0644);
			if (fd == -1)
				return ((*env_list)->value = ft_strdup("1"), perror(""), -1);
			close(fd);
			last = file;
		}
		if (file->next && file->next->type != 1 && file->next->type != 2)
			return (perror(""), (*env_list)->value = ft_strdup("1"), -1);
		file = file->next;
	}
	if (last)
		cmd->inoutfile = last;
	return (1);
}

int	is_redirection(t_command *cmd, t_list_env **env_list, char ***env,
		char ***env1, char *file)
{
	t_command	*cmd2;

	int(fd), (flags), (saved_stdout);
	saved_stdout = dup(1);
	cmd2 = cmd;
	if (cmd2->inoutfile && (cmd2->inoutfile->type == 1
			|| cmd2->inoutfile->type == 2))
	{
		flags = get_flags(cmd2);
		if (multiple_out(&cmd2, flags, env_list, env, env1, file) != 1)
			return (1);
		fd = open(cmd2->inoutfile->filename, flags, 0644);
		if (fd == -1)
			return (perror("fd error :"), (*env_list)->value = "1", 1);
		if (dup2(fd, 1) == -1)
			return (perror("dup2 failed"), (*env_list)->value = "1", close(fd),
				1);
		close(fd);
		execute_cmd(cmd, env_list, env, env1);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		return (1);
	}
	else if (cmd2->inoutfile && (cmd2->inoutfile->type == 0
			|| cmd2->inoutfile->type == 3))
		return (in_heredoc_redirs(cmd, env_list, env, env1, file), 1);
	return (0);
}

int multi_redir(t_command *cmd, t_list_env **env_list, char ***env,
		char ***env1, char *file)
{
	t_command	*cmd2;

	int(fd), (flags), (saved_stdout);
	saved_stdout = dup(1);
	cmd2 = cmd;
	if (cmd2->inoutfile && (cmd2->inoutfile->type == 1
			|| cmd2->inoutfile->type == 2))
	{
		flags = get_flags(cmd2);
		if (multiple_out(&cmd2, flags, env_list, env, env1, file) != 1)
			return (1);
		fd = open(cmd2->inoutfile->filename, flags, 0644);
		if (fd == -1)
			return (perror("fd error :"), (*env_list)->value = "1", 1);
		if (dup2(fd, 1) == -1)
			return (perror("dup2 failed"), (*env_list)->value = "1", close(fd),
				1);
		close(fd);
		printf("%s", cmd->args[0]);
		execute_cmd(cmd, env_list, env, env1);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		return (1);
	}
	return 100;
}

void	heredoc_redirection(struct s_command *cmd, t_list_env **env_list,
		char ***env, char ***env1, char *file)
{
	t_command *cmd2 = cmd;
	while (cmd2->inoutfile && cmd2->inoutfile->type == 3)
		cmd2->inoutfile = cmd2->inoutfile->next;
	if (cmd2->inoutfile)
		multi_redir(cmd2, env_list, env, env1, file);
	else
	{
		int(fd), (saved_stdout);
		saved_stdout = dup(0);
		if (ft_strcmp(cmd->heredoc, "ctrlC") == 0)
			return ;
		fd = open(file, O_RDONLY);
		if (fd == -1)
			return ((*env_list)->value = ft_strdup("1"), perror(""));
		dup2(fd, 0);
		execute_cmd(cmd, env_list, env, env1);
		close(fd);
		dup2(saved_stdout, 0);
		close(saved_stdout);
	}
	return ;
}

void	in_heredoc_redirs(struct s_command *cmd, t_list_env **env_list,
		char ***env, char ***env1, char *file)
{
	int(fd), (saved_stdout);
	saved_stdout = dup(0);
	if (ft_strcmp(cmd->inoutfile->filename, "ctrlC") == 0)
		return ;
	if (cmd->inoutfile->type == 0)
	{
		while (cmd->inoutfile->next && cmd->inoutfile->type == 0 && cmd->inoutfile->next->type == 0)
			cmd->inoutfile = cmd->inoutfile->next;
		printf("%s", cmd->inoutfile->filename);
		fd = open(cmd->inoutfile->filename, O_RDONLY);
		if (fd == -1)
			return ((*env_list)->value = ft_strdup("1"), perror(""));
		dup2(fd, 0);
		execute_cmd(cmd, env_list, env, env1);
		close(fd);
		dup2(saved_stdout, 0);
		close(saved_stdout);
		if(cmd->inoutfile->next && cmd->inoutfile->type == 1)
		{
			is_redirection(cmd, env_list, env, env1, file);
		}
		return ;
	}
	else
		heredoc_redirection(cmd, env_list, env, env1, file);
}

int	ft_size1(t_command *list)
{
	int i;

	i = 0;
	while(list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

void	execute_piped_commands(t_command *cmd_list, t_list_env **env_list,
		char ***env, char ***env1)
{
	pid_t	*pid;
	int		status;
	int		n_cmd;
	char	*file;
	int		i;

	status = 0;
	i = 0;
	n_cmd = ft_size1(cmd_list);
	pid = ft_malloc(sizeof(pid_t) * n_cmd, 0);
	file = cmd_list->heredoc;
	int(pipe_fd[2]), (prev_fd);
	prev_fd = -1;
	if (!cmd_list->next)
	{
		if (is_redirection(cmd_list, env_list, env, env1, file) != 1)
			execute_cmd(cmd_list, env_list, env, env1);
		return ;
	}
	while (cmd_list)
	{
		file = cmd_list->heredoc;
		if (cmd_list->next)
		{
			if (pipe(pipe_fd) == -1)
			return ((*env_list)->value = ft_strdup("1"), perror("pipe"));
		}
		pid[i] = fork();
		if (pid[i] == -1)
			return ((*env_list)->value = ft_strdup("1"), perror("fork"));
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
			if (is_redirection(cmd_list, env_list, env, env1, file) == 1)
				exit(1);
			execute_cmd(cmd_list, env_list, env, env1);
				exit(ft_atoi((*env_list)->value));
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd_list->next)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			cmd_list = cmd_list->next;
		}
		i++;
	}
	i = 0;
	while (i < n_cmd)
	{
		waitpid(pid[i], &status, 0);
		if (i == n_cmd - 1)
		{
			if (WIFEXITED(status))
				(*env_list)->value = ft_itoa(WEXITSTATUS(status));
		}
		i++;
	}
}
