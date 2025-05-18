/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/18 12:04:58 by obarais          ###   ########.fr       */
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

void	shell_luncher(t_command *cmdList, char **env, t_list_env **env_list)
{
	pid_t	pid;
	int		status;
	// int		sig;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (execve_like_execvp(cmdList->args[0], cmdList->args, env) == -1)
			write(2, "command not found\n", 19);
		(*env_list)->value = ft_strdup("127");
		exit(127);
	}
	else if (pid < 0)
	{
		perror("Error");
		(*env_list)->value = ft_strdup("1");
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)){
			int exit_code = WEXITSTATUS(status);
			(*env_list)->value = ft_itoa(exit_code);
			// printf("\n%d\n", exit_code);
		}
		if (WIFSIGNALED(status))
        {
            int sig = WTERMSIG(status);
            if (sig == SIGINT)
				write(1, "\n", 1);
			else if (sig == SIGQUIT){
				write(2, "Quit (core dumped)\n", 20);
				(*env_list)->value = ft_itoa(128 + sig);
			}
        }
		signal(SIGINT, sigint_handler);
	}
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

t_list_env *ft_getenv(t_list_env **env_list, char *str)
{
	t_list_env *list;

	list = *env_list;
	while(list)
	{
		if (ft_strcmp((list)->key, str) == 0)
			return (list);
		list = (list)->next;
	}
	return (NULL);
}
char *change_dir(t_list_env **env_list)
{
	char *cwd = malloc(1024);
	if (!cwd)
		return (NULL);

	if (getcwd(cwd, 1024) == NULL)
	{
		(*env_list)->value = ft_strdup("1");
		perror("getcwd() error");
		free(cwd);
		return (NULL);
	}
	return (cwd);
}
void	ft_cd(char **cmdlist, t_list_env **env_list)
{
	if (!cmdlist[1])
	{
		write(2, "cd: missing argument\n", 22);
		(*env_list)->value = "1";
		return ;
	}
	if (cmdlist[2])
	{
		write(2, "too many arguments\n", 20);
		(*env_list)->value = "1";
		return ;
	}
	if (ft_strcmp(cmdlist[1], "~") == 0)
		chdir(getenv("HOME"));
	else if (chdir(cmdlist[1]) != 0)
	{
		(*env_list)->value = "1";
		return (write(1, "cd: ", 4), perror(""));
	}
	ft_getenv(env_list, "OLDPWD")->value = ft_strdup(
	ft_getenv(env_list, "PWD")->value);
	ft_getenv(env_list, "PWD")->value = ft_strdup(change_dir(env_list));
	(*env_list)->value = "0";
}

int is_number(char *str)
{
	int i;

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

void ft_exit(char **args, t_list_env **list)
{
	int i;

	if (!args[1])
	{
		write(2, "exit\n", 5);
		exit(ft_atoi((*list)->value));
	}
	if(args[1] && args[2])
	{
		write(2, "exit\ntoo many arguments\n", 25);
		(*list)->value = ft_strdup("1");
		return ;
	}
	if (is_number(args[1]))
	{
		i = ft_atoi(args[1]) % 256;
		write(1, "exit\n", 5);
		exit(i);
	}
	else
	{
		write(2, "exit\n", 5);
		write(2, "numeric argument required\n", 26);
		exit(2);
	}
	(*list)->value = ft_strdup("2");
}

void	execute_cmd(t_command *cmd_list, t_list_env **env_list, char ***env,
		char ***env1)
{
	if (ft_strcmp(cmd_list->args[0], "pwd") == 0)
		getworkingdir(env_list);
	else if (ft_strcmp(cmd_list->args[0], "env") == 0)
		getenvfunc(*env, env_list, cmd_list->args);
	else if (ft_strcmp(cmd_list->args[0], "echo") == 0)
		ft_echo(cmd_list->args, env_list);
	else if (ft_strcmp(cmd_list->args[0], "cd") == 0)
		ft_cd(cmd_list->args, env_list);
	else if (ft_strcmp(cmd_list->args[0], "export") == 0)
		ft_export(cmd_list->args, *env1, env_list);
	else if (ft_strcmp(cmd_list->args[0], "unset") == 0)
		ft_unset(cmd_list->args, env, env_list);
	else if (ft_strcmp(cmd_list->args[0], "exit") == 0)
		ft_exit(cmd_list->args, env_list);
	else
	{
		shell_luncher(cmd_list, *env, env_list);
	}
}

void	exection(t_command *cmd_list, t_list_env **env_list)
{
	char	**env;
	char	**env1;

	env = list_to_table(*env_list);
	env1 = list_to_table_export(*env_list);
	execute_piped_commands(cmd_list, env_list, &env, &env1);
}
