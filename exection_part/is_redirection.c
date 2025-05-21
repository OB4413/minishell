/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/21 15:00:59 by obarais          ###   ########.fr       */
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

// void	execute_piped_commands(t_command *cmd_list, t_list_env **env_list)
// {
// 	pid_t	*tab;
// 	int		n_cmd;
// 	char	*file;

// 	int status, i;
// 	n_cmd = ft_lstsize1(cmd_list);
// 	status = 0;
// 	i = 0;
// 	tab = ft_malloc(sizeof(pid_t) * n_cmd, 0);
// 	file = cmd_list->heredoc;
// 	int(pipe_fd[2]), (prev_fd);
// 	prev_fd = -1;
// 	if (!cmd_list->next)
// 	{
// 		if (before_redir(cmd_list, env_list, file) != 1)
// 			execute_cmd(cmd_list, env_list);
// 		// exit(0);
// 		return ;
// 	}
// 	while (cmd_list)
// 	{
// 		file = cmd_list->heredoc;
// 		if (cmd_list->next)
// 		{
// 			if (pipe(pipe_fd) == -1)
// 				return ((*env_list)->value = ft_strdup("1"), perror("pipe"));
// 		}
// 		tab[i] = fork();
// 		if (tab[i] == -1)
// 			return ((*env_list)->value = ft_strdup("1"), perror("fork"));
// 		if (tab[i] == 0)
// 		{
// 			if (prev_fd != -1)
// 			{
// 				dup2(prev_fd, 0);
// 				close(prev_fd);
// 			}
// 			if (cmd_list->next)
// 			{
// 				dup2(pipe_fd[1], 1);
// 				close(pipe_fd[0]);
// 				close(pipe_fd[1]);
// 			}
// 			if (before_redir(cmd_list, env_list, file) != 1)
// 				execute_cmd(cmd_list, env_list);
// 			exit(ft_atoi((*env_list)->value));
// 		}
// 		else
// 		{
// 			if (prev_fd != -1)
// 				close(prev_fd);
// 			if (cmd_list->next)
// 			{
// 				close(pipe_fd[1]);
// 				prev_fd = pipe_fd[0];
// 			}
// 			cmd_list = cmd_list->next;
// 		}
// 		i++;
// 	}
// 	i = 0;
// 	while (i < n_cmd)
// 	{
// 		waitpid(tab[i], &status, 0);
// 		if (i == n_cmd - 1)
// 		{
// 			if (WIFEXITED(status))
// 				(*env_list)->value = ft_itoa(WEXITSTATUS(status));
// 		}
// 		i++;
// 	}
// }

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

void	exec_child(t_command *cmd, t_list_env **env_list,
					int prev_fd, int pipe_fd[2])
{
	char	*file;

	file = cmd->heredoc;
	if (prev_fd != -1)
	{
		dup2(prev_fd, 0);
		close(prev_fd);
	}
	if (cmd->next)
	{
		dup2(pipe_fd[1], 1);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (before_redir(cmd, env_list, file) != 1)
		execute_cmd(cmd, env_list);
	ft_exit_status(ft_atoi((*env_list)->value));
}

void	exec_parent(t_command *cmd, int *prev_fd, int pipe_fd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}

void	wait_all(pid_t *pids, int n_cmd, t_list_env **env_list)
{
	int	status;
	int	i;

	i = 0;
	while (i < n_cmd)
	{
		waitpid(pids[i], &status, 0);
		if (i == n_cmd - 1 && WIFEXITED(status))
			(*env_list)->value = ft_itoa(WEXITSTATUS(status));
		i++;
	}
}

void	execute_piped_commands(t_command *cmd_list, t_list_env **env_list)
{
	pid_t	*pids;

	int (n_cmd), (i), (pipe_fd[2]), (prev_fd);
	i = 0;
	prev_fd = -1;
	n_cmd = ft_lstsize1(cmd_list);
	pids = ft_malloc(sizeof(pid_t) * n_cmd, 0);
	if (!cmd_list->next)
		return (execute_single(cmd_list, env_list));
	while (cmd_list)
	{
		setup_pipe(cmd_list, pipe_fd);
		pids[i] = fork();
		if (pids[i] == -1)
		{
			(*env_list)->value = ft_strdup("1");
			return (perror("fork"));
		}
		if (pids[i] == 0)
			exec_child(cmd_list, env_list, prev_fd, pipe_fd);
		exec_parent(cmd_list, &prev_fd, pipe_fd);
		cmd_list = cmd_list->next;
		i++;
	}
	wait_all(pids, n_cmd, env_list);
}
