/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/21 15:31:57 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	getworkingdir(t_list_env **list)
{
	printf("%s\n", ft_getenv(list, "PWD")->value);
	(*list)->value = ft_strdup("0");
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
