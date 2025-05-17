/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/17 11:14:49 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	getworkingdir(t_list_env **list)
{

	printf("%s\n", ft_getenv(list, "PWD")->value);

}

void	getenvfunc(char **env, t_list_env **env_list)
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
            int sig = WTERMSIG(status);
            if (sig == SIGINT)
                printf("\n");
			else if (sig == SIGQUIT)
				printf("Quit (core dumped)\n");
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
		if (cmdlist[i + 1] && cmdlist[i + 1][0])
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
char *change_dir()
{
	char *cwd = malloc(1024);
	if (!cwd)
		return (NULL);

	if (getcwd(cwd, 1024) == NULL)
	{
		perror("getcwd() error");
		free(cwd);
		return (NULL);
	}
	// printf("\n%s\n", cwd);
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
		write(2, "cd: too many arguments\n", 22);
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
	ft_getenv(env_list, "OLDPWD")->value = ft_strdup(
	ft_getenv(env_list, "PWD")->value);
	ft_getenv(env_list, "PWD")->value = ft_strdup(change_dir());
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
		exit(255);
	}
	(*list)->value = ft_strdup("2");
}

void	execute_cmd(t_command *cmd_list, t_list_env **env_list, char ***env,
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
