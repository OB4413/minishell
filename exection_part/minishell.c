/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/11 18:03:19 by obarais          ###   ########.fr       */
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
	// list_to_table(env_list);

	int	i;
	i = 0;
	while (env[i] != NULL)
	{
		if(ft_strchr(env[i], '=') && ((ft_strchr(env[i], '=') + 1)[1] != '\0'))
			printf("%s\n", env[i]);
		i++;
	}
	(*env_list)->value = "0";
}

void	shell_luncher(t_command *cmdList, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve_like_execvp(cmdList->args[0] , cmdList->args, env) == -1)
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
		if (WIFSIGNALED(status))
        {
            int sig = WTERMSIG(status);
            if (sig == SIGINT)
                printf("\n");
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
		return(perror("Error "));
	}
	(*env_list)->value = "0";
}

void execute_cmd(t_command *cmd_list, list_env **env_list, char ***env, char ***env1)
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
		shell_luncher(cmd_list, *env);
	}
}

void	exection(t_command *cmd_list, list_env **env_list)
{
	char **env, **env1;
	// int c;
	// list_env *list;
	// list = *env_list;
	// while(list)
	// {
	// 	if(ft_strcmp(list->key, "SHLVL") == 0)
	// 	{
	// 		c = ft_atoi(list->value);
	// 		c++;
	// 		// free(list->value);
	// 		list->value = ft_strdup(ft_itoa(c));
	// 	}
	// 	list = list->next;
	// }

	env = list_to_table(*env_list);
	env1 = list_to_table_export(*env_list);
	// int j = 1;
	// t_command *cmd_list2 = cmd_list;
	// while (cmd_list2)
	// {
	//         printf("command %d:\n", j);
	//         printf("cmd :%s\n", cmd_list2);
	//         printf("args :");
	//         for (size_t i = 0; cmd_list2->args[i]; i++)
	//         {
	//             printf("%s  ", cmd_list2->args[i]);
	//         }
	//         printf("\n");
	//         while(cmd_list2->inoutfile)
	//         {
	//             printf("filename :%s   type:%d\n",  cmd_list2->inoutfile->filename, cmd_list2->inoutfile->type);
	//             cmd_list2->inoutfile = cmd_list2->inoutfile->next;
	//         }
	//         cmd_list2 = cmd_list2->next;
	//         j++;
	// }
	execute_piped_commands(cmd_list, env_list, &env, &env1);
	// if(is_redirection(cmd_list, env_list, &env) == 1)
	// {
	// 	printf("ccc");
	// 	// exit(0);
	// 	return ;
	// }
	// execute_cmd(cmd_list, env_list, &env);
}
