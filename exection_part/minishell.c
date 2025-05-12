/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/10 17:00:52 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	getworkingdir(list_env **list)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		(*list)->value = "0";
	}
	else
	{
		perror("getcwd() error");
		(*list)->value = "1";
	}
}

void	getenvfunc(char **env, list_env **env_list)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strchr(env[i], '=') && ((ft_strchr(env[i], '=') + 1)[1] != '\0'))
			printf("%s\n", env[i]);
		i++;
	}
	(*env_list)->value = "0";
}

void	shell_luncher(t_command *cmdList, char **env, list_env **env_list)
{
	pid_t	pid;
	int		status;
	int		sig;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (execve_like_execvp(cmdList->args[0], cmdList->args, env) == -1)
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
		if (WIFEXITED(status))
        	(*env_list)->value = ft_itoa(WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			printf("%d", WTERMSIG(status));
			sig = WTERMSIG(status);
			if (sig == SIGINT)
				printf("\n");
			else if (sig == SIGQUIT)
				printf("Quit (core dumped)\n");
		}
		signal(SIGINT, sigint_handler);
	}
}

void	ft_echo(char **cmdlist, list_env **env_list)
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
		printf("%s ", cmdlist[i]);
		i++;
	}
	if (flag)
		printf("\n");
	(*env_list)->value = "0";
}

void	ft_cd(char **cmdlist, list_env **env_list)
{
	if (!cmdlist[1])
	{
		printf("cd: missing argument\n");
		(*env_list)->value = "1";
		return ;
	}
	if (cmdlist[2])
	{
		printf("cd: too many arguments\n");
		(*env_list)->value = "1";
		return ;
	}
	if (ft_strcmp(cmdlist[1], "~") == 0)
		chdir(getenv("HOME"));
	else if (chdir(cmdlist[1]) != 0)
	{
		(*env_list)->value = "1";
		return (write(1, "cd: ", 4), write(1, cmdlist[1],
		ft_strlen(cmdlist[1])), write(1, ": ", 2) , perror(""));
	}
	(*env_list)->value = "0";
}

void	execute_cmd(t_command *cmd_list, list_env **env_list, char ***env,
		char ***env1)
{
	if (ft_strcmp(cmd_list->args[0], "pwd") == 0)
		getworkingdir(env_list);
	else if (ft_strcmp(cmd_list->args[0], "env") == 0)
		getenvfunc(*env, env_list);
	else if (ft_strcmp(cmd_list->args[0], "echo") == 0)
		ft_echo(cmd_list->args, env_list);
	else if (ft_strcmp(cmd_list->args[0], "cd") == 0)
		ft_cd(cmd_list->args, env_list);
	else if (ft_strcmp(cmd_list->args[0], "export") == 0)
		ft_export(cmd_list->args, *env1, env_list);
	else if (ft_strcmp(cmd_list->args[0], "unset") == 0)
		ft_unset(cmd_list->args, env, env_list);
	else
	{
		shell_luncher(cmd_list, *env, env_list);
	}
}

void	exection(t_command *cmd_list, list_env **env_list)
{
	char	**env;
	char	**env1;

	env = list_to_table(*env_list);
	env1 = list_to_table_export(*env_list);
	execute_piped_commands(cmd_list, env_list, &env, &env1);
}
