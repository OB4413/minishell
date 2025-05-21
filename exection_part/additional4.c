/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:32:57 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/21 15:35:13 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

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
