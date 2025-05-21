/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtings.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:45:17 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/21 15:05:45 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

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

t_list_env	*ft_getenv(t_list_env **env_list, char *str)
{
	t_list_env	*list;

	list = *env_list;
	while (list)
	{
		if (ft_strcmp((list)->key, str) == 0)
			return (list);
		list = (list)->next;
	}
	return (NULL);
}

char	*change_dir(t_list_env **env_list)
{
	char	*cwd;

	cwd = ft_malloc(1024, 0);
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
	ft_getenv(env_list, "OLDPWD")->value = ft_strdup(ft_getenv(env_list,
			"PWD")->value);
	ft_getenv(env_list, "PWD")->value = ft_strdup(change_dir(env_list));
	(*env_list)->value = "0";
}

void	ft_exit_status(int n)
{
	ft_malloc(0, 1);
	exit(n);
}

void	ft_exit(char **args, t_list_env **list)
{
	int	i;

	if (!args[1])
	{
		write(2, "exit\n", 5);
		ft_exit_status(ft_atoi((*list)->value));
	}
	if (args[1] && args[2])
	{
		write(2, "exit\ntoo many arguments\n", 25);
		(*list)->value = ft_strdup("1");
		return ;
	}
	if (is_number(args[1]))
	{
		i = ft_atoi(args[1]) % 256;
		write(1, "exit\n", 5);
		ft_exit_status(i);
	}
	else
	{
		write(2, "exit\n", 5);
		return (write(2, "numeric argument required\n", 26), ft_exit_status(2));
	}
	(*list)->value = ft_strdup("2");
}
