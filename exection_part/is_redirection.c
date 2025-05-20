/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/20 15:23:11 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	get_flags(int type)
{
	int	flags;

	if (type == 1)
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

int before_redir(t_command *cmd, t_list_env **env_list, char ***env,
		char ***env1, char *file)
{
	int (saved_stdout), (saved_stdin);

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (is_redirection(cmd->inoutfile, env_list, file) >= 0)
		execute_cmd(cmd, env_list, env, env1);
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	return 1;
}

int	handle_output_redirection(t_redir *redir, t_list_env **env)
{
	int (flags), (fd);
	flags = get_flags(redir->type);
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		(*env)->value = ft_strdup("126");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("minishell: dup2 failed\n", 2);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_input_redirection(t_redir *redir, t_list_env **env)
{
	int fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		(*env)->value = ft_strdup("1");
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ft_putstr_fd("minishell: dup2 failed\n", 2), (*env)->value = ft_strdup("1"), close(fd), -1);
	close(fd);
	return (0);
}

int	is_redirection(t_redir *redir, t_list_env **env_list, char *file)
{
	t_redir *redirection;

	redirection = redir;
	while (redirection)
	{
		if (redirection->type == 1 || redirection->type == 2) // > or >>
		{
			if (handle_output_redirection(redirection, env_list) < 0)
			return (-1);
		}
		else if (redirection->type == 0) // <
		{
			if (handle_input_redirection(redirection, env_list) < 0)
			return (-1);
		}
		else if (redirection->type == 3) // <<
		{
			if (heredoc_redirection(redir, file, env_list) == -1)
				return (-1); // Only sets stdin
		}
		redirection = redirection->next;
	}
	return 0;
}

int	heredoc_redirection(t_redir *redir, char *file, t_list_env **env_list)
{
	int fd;
	if (!file || ft_strcmp(file, "ctrlC") == 0)
		return 0;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (perror("heredoc open"), (*env_list)->value = ft_strdup("1"), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), (*env_list)->value = ft_strdup("1"), close(fd), -1);
	close(fd);
	while (redir && redir->type == 3)
		redir = redir->next;
	if (!redir)
		return (-1);
	return 0;
}

int ft_lstsize1(t_command *lst)
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


void	execute_piped_commands(t_command *cmd_list, t_list_env **env_list,
		char ***env, char ***env1)
{
	int		status, i;
	pid_t *tab;
	int		n_cmd = ft_lstsize1(cmd_list);
	char	*file;

	status = 0;
	i = 0;
	tab = ft_malloc(sizeof( pid_t) * n_cmd, 0);
	file = cmd_list->heredoc;
	int(pipe_fd[2]), (prev_fd);
	prev_fd = -1;
	if (!cmd_list->next)
	{
		if (before_redir(cmd_list, env_list, env, env1, file) != 1)
			execute_cmd(cmd_list, env_list, env, env1);
		// exit(0);
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
		tab[i] = fork();
		if (tab[i] == -1)
			return ((*env_list)->value = ft_strdup("1"), perror("fork"));
		if (tab[i] == 0)
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
			if (before_redir(cmd_list, env_list, env, env1, file) != 1)
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
	while ( i < n_cmd)
	{
		waitpid(tab[i], &status, 0);
		if (i ==  n_cmd - 1)
		{
			if (WIFEXITED(status))
				(*env_list)->value = ft_itoa(WEXITSTATUS(status));
		}
		i++;
	}

}
