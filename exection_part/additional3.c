/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:30:40 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/21 15:32:36 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	exec_fork_error(t_list_env **env_list)
{
	perror("Error");
	(*env_list)->value = ft_strdup("1");
}

void	exec_parent1(pid_t pid, t_list_env **env_list)
{
	int (status), (sig);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*env_list)->value = ft_itoa(WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			(*env_list)->value = ft_itoa(130);
			write(1, "\n", 1);
		}
		else if (sig == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 20);
			(*env_list)->value = ft_itoa(131);
		}
	}
	signal(SIGINT, sigint_handler);
}

void	getenvfunc(char **env, t_list_env **env_list, char **args)
{
	int	i;

	i = 0;
	if (args[1])
	{
		write(2, "No such file or directory\n", 27);
		(*env_list)->value = "1";
		return ;
	}
	while (env[i] != NULL)
	{
		if (ft_strchr(env[i], '=') && ((ft_strchr(env[i], '=')[1]) != '\0'))
			printf("%s\n", env[i]);
		i++;
	}
	(*env_list)->value = "0";
}

void	ft_echo(char **cmdlist, t_list_env **env_list)
{
	bool	flag;
	int		i;

	flag = true;
	i = 1;
	if (cmdlist[1] && ft_strcmp(cmdlist[1], "-n") == 0)
	{
		flag = false;
		i++;
	}
	while (cmdlist[i])
	{
		printf("%s", cmdlist[i]);
		if (cmdlist[i + 1])
			printf(" ");
		i++;
	}
	if (flag)
		printf("\n");
	(*env_list)->value = "0";
}
