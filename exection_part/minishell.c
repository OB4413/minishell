/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/21 12:22:25 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	getworkingdir(t_list_env **list)
{
	printf("%s\n", ft_getenv(list, "PWD")->value);
	(*list)->value = ft_strdup("0");
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

// void	shell_luncher(t_command *cmdList, char **env, t_list_env **env_list)
// {
// 	pid_t	pid;

// 	int (status), (exit_code), (sig);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		signal(SIGQUIT, SIG_DFL);
// 		if (execve_like_execvp(cmdList->args[0], cmdList->args, env,
// 				env_list) == -1)
// 			write(2, "command not found\n", 19);
// 		(*env_list)->value = ft_strdup("127");
// 		exit(127);
// 	}
// 	else if (pid < 0)
// 	{
// 		perror("Error");
// 		(*env_list)->value = ft_strdup("1");
// 	}
// 	else
// 	{
// 		signal(SIGINT, SIG_IGN);
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 		{
// 			exit_code = WEXITSTATUS(status);
// 			(*env_list)->value = ft_itoa(exit_code);
// 		}
// 		if (WIFSIGNALED(status))
// 		{
// 			sig = WTERMSIG(status);
// 			if (sig == SIGINT)
// 			{
// 				(*env_list)->value = ft_itoa(130);
// 				write(1, "\n", 1);
// 			}
// 			else if (sig == SIGQUIT)
// 			{
// 				write(2, "Quit (core dumped)\n", 20);
// 				(*env_list)->value = ft_itoa(131);
// 			}
// 		}
// 		signal(SIGINT, sigint_handler);
// 	}
// }

void	exec_child1(t_command *cmdList, char **env, t_list_env **env_list)
{
	signal(SIGQUIT, SIG_DFL);
	if (execve_like_execvp(cmdList->args[0],
			cmdList->args, env, env_list) == -1)
		write(2, "command not found\n", 19);
	(*env_list)->value = ft_strdup("127");
	exit(127);
}

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

void	shell_luncher(t_command *cmdList, char **env, t_list_env **env_list)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		exec_child1(cmdList, env, env_list);
	else if (pid < 0)
		exec_fork_error(env_list);
	else
		exec_parent1(pid, env_list);
}

int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	execute_cmd(t_command *cmd_list, t_list_env **env_list)
{
	char	**env;
	char	**env1;

	env = list_to_table(*env_list);
	env1 = list_to_table_export(*env_list);
	if (ft_strcmp(cmd_list->args[0], "pwd") == 0)
		getworkingdir(env_list);
	else if (ft_strcmp(cmd_list->args[0], "env") == 0)
		getenvfunc(env, env_list, cmd_list->args);
	else if (ft_strcmp(cmd_list->args[0], "echo") == 0)
		ft_echo(cmd_list->args, env_list);
	else if (ft_strcmp(cmd_list->args[0], "cd") == 0)
		ft_cd(cmd_list->args, env_list);
	else if (ft_strcmp(cmd_list->args[0], "export") == 0)
		ft_export(cmd_list->args, env1, env_list);
	else if (ft_strcmp(cmd_list->args[0], "unset") == 0)
		ft_unset(cmd_list->args, &env, env_list);
	else if (ft_strcmp(cmd_list->args[0], "exit") == 0)
		ft_exit(cmd_list->args, env_list);
	else if (cmd_list->args[0])
		shell_luncher(cmd_list, env, env_list);
}

void	exection(t_command *cmd_list, t_list_env **env_list)
{
	if (!cmd_list)
		return ;
	execute_piped_commands(cmd_list, env_list);
}
