/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:03:27 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/20 22:36:33 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	ft_lstadd_back2(t_list_env **lst, t_list_env *new)
{
	t_list_env	*last;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast2(*lst);
	last->next = new;
}

int	get_flags(int type)
{
	int	flags;

	if (type == 1)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	return (flags);
}

int	before_redir(t_command *cmd, t_list_env **env_list, char *file)
{
	int (saved_stdout), (saved_stdin);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (is_redirection(cmd->inoutfile, env_list, file) >= 0)
		execute_cmd(cmd, env_list);
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
	return (1);
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
	int	fd;

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
		return (ft_putstr_fd("minishell: dup2 failed\n", 2),
			(*env)->value = ft_strdup("1"), close(fd), -1);
	close(fd);
	return (0);
}
