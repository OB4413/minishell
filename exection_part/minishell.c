/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/04 16:34:23 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	getworkingdir(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}

void	getenvfunc(char **env)
{
	// list_to_table(env_list);

	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	shell_luncher(t_command *cmdList)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve_like_execvp(cmdList->cmd , cmdList->args) == -1)
			perror("Error");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("Error");
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, sigint_handler);
	}
}

void	ft_echo(char **cmdlist)
{
	bool	flag;
	int		i;

	flag = true;
	i = 0;
	if (cmdlist[0] && ft_strcmp(cmdlist[0], "-n") == 0)
	{
		flag = false;
		i++;
	}
	while (cmdlist[i])
	{
		printf("%s ", cmdlist[i]);
		i++;
	}
	if (flag)
		printf("\n");
}

void	ft_cd(char **cmdlist)
{
	if (!cmdlist[0])
	{
		printf("cd: missing argument\n");
		return ;
	}
	if (cmdlist[1])
	{
		printf("cd: too many arguments\n");
		return ;
	}
	if (ft_strcmp(cmdlist[0], "~") == 0)
		chdir(getenv("HOME"));
	else if (chdir(cmdlist[0]) != 0)
	{
		perror("Error ");
	}
}

void	exection(t_command *cmd_list, list_env **env_list)
{
	char **env;
	env = list_to_table(*env_list);

	if (ft_strcmp(cmd_list->cmd, "pwd") == 0)
		getworkingdir();
	else if (ft_strcmp(cmd_list->cmd, "env") == 0)
		getenvfunc(env);
	else if (ft_strcmp(cmd_list->cmd, "echo") == 0)
		ft_echo(cmd_list->args);
	else if (ft_strcmp(cmd_list->cmd, "cd") == 0)
		ft_cd(cmd_list->args);
	else if (ft_strcmp(cmd_list->cmd, "export") == 0)
		ft_export(cmd_list->args, &env);
	else if (ft_strcmp(cmd_list->cmd, "unset") == 0)
		ft_unset(cmd_list->args, &env);
	else
	{
		shell_luncher(cmd_list);
	}
	free(cmd_list->cmd);
}
