/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/06 14:57:32 by obarais          ###   ########.fr       */
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
		if(ft_strchr(env[i], '=') && ((ft_strchr(env[i], '=') + 1)[1] != '\0'))
			printf("%s\n", env[i]);
		i++;
	}
	printf("this is from env");
}

void	shell_luncher(t_command *cmdList, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (execve_like_execvp(cmdList->cmd , cmdList->args, env) == -1)
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
            else if (sig == SIGQUIT)
                printf("Quit (core dumped)\n");
        }
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

void execute_cmd(t_command *cmd_list, list_env **env_list, char ***env)
{
	// int i = 0;
	// printf("hhhhhhhhhhhhhh");
	// while(*(env)[i])
	// 	printf("%s\n", *(env)[i++]);
	if (ft_strcmp(cmd_list->cmd, "pwd") == 0)
		getworkingdir();
	else if (ft_strcmp(cmd_list->cmd, "env") == 0)
		getenvfunc(*env);
	else if (ft_strcmp(cmd_list->cmd, "echo") == 0)
		ft_echo(cmd_list->args);
	else if (ft_strcmp(cmd_list->cmd, "cd") == 0)
		ft_cd(cmd_list->args);
	else if (ft_strcmp(cmd_list->cmd, "export") == 0)
		ft_export(cmd_list->args, *env, env_list);
	else if (ft_strcmp(cmd_list->cmd, "unset") == 0)
		ft_unset(cmd_list->args, env, env_list);
	else
	{
		shell_luncher(cmd_list, *env);
	}
	// free(cmd_list->cmd);
}

void	exection(t_command *cmd_list, list_env **env_list)
{
	char **env;
	env = list_to_table(*env_list);

	// int j = 1;
	// t_command *cmd_list2 = cmd_list;
	// while (cmd_list2)
	// {
	//         printf("command %d:\n", j);
	//         printf("cmd :%s\n", cmd_list2->cmd);
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
	if(is_redirection(cmd_list, env_list, &env) == 1)
	{
		printf("ccc");
		// exit(0);
		return ;
	}
	execute_cmd(cmd_list, env_list, &env);
}
