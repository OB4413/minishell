/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:56:34 by obarais           #+#    #+#             */
/*   Updated: 2025/05/06 14:49:09 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	ft_list_env(char **env, list_env **env_list)
{
	int i;
	int j;
	list_env *new_env;
	list_env *tmp;

	i = 0;
	j = 0;
    new_env = (list_env *)malloc(sizeof(list_env));
    new_env->key = ft_strdup("$?");
    new_env->value = "0";
    new_env->next = NULL;
    *env_list = new_env;
	while (env[i])
    {
        j = 0;
        while (env[i][j] != '=')
            j++;
        new_env = (list_env *)malloc(sizeof(list_env));
        if (!new_env)
            return ;
        new_env->key = ft_substr(env[i], 0, j);
        new_env->value = ft_substr(env[i], j + 1, ft_strlen(env[i]) - j);
        new_env->equal = 1;
        new_env->next = NULL;
        if (*env_list == NULL)
            *env_list = new_env;
        else
        {
            tmp = *env_list;
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new_env;
        }
        i++;
    }
}

char **put_the_args(t_input *tok, char *cmd)
{
    char **args;
    int i = 0;
    int j = 0;
    t_input *tmp2;
    t_input *tmp;

    tmp = tok;
    while (strcmp(tmp->value, cmd) != 0)
        tmp = tmp->next;
    tmp = tmp->next;
    tmp2 = tmp;
    while(tmp2 && tmp2->type != PIPE)
    {
        if (tmp2->type == HEREDOC || tmp2->type == APPEND || tmp2->type == REDIRECT_IN || tmp2->type == REDIRECT_OUT)
        {
            tmp2 = tmp2->next;
            tmp2 = tmp2->next;
            continue;
        }
        i++;
        tmp2 = tmp2->next;
    }
    args = (char **)malloc(sizeof(char *) * (i + 1));
    if (args == NULL)
        return (NULL);
    while (tmp  && tmp->type != PIPE)
    {
        if (tmp->type == HEREDOC || tmp->type == APPEND || tmp->type == REDIRECT_IN || tmp->type == REDIRECT_OUT)
        {
            tmp = tmp->next;
            tmp = tmp->next;
            continue;
        }
        args[j] = ft_strdup(tmp->value);
        j++;
        tmp = tmp->next;
    }
    args[j] = NULL;
    return (args);
}

int what_direction(char *str)
{
    if (strcmp(str, "<") == 0)
        return 0;
    else if (strcmp(str, ">") == 0)
        return 1;
    else if (strcmp(str, ">>") == 0)
        return 2;
    else if (strcmp(str, "<<") == 0)
        return 3;
    return -1;
}

t_redir *check_derctions(t_input *tok, char *cmd)
{
    t_redir *redir = NULL;
    t_input *tmp;
    t_redir *new_redir;
    t_redir *tmp2;

    tmp = tok;
    while (strcmp(tmp->value, cmd) != 0)
        tmp = tmp->next;
    tmp = tmp->next;
    while (tmp && tmp->type != PIPE)
    {
        if (tmp->type == HEREDOC || tmp->type == APPEND || tmp->type == REDIRECT_IN || tmp->type == REDIRECT_OUT)
        {
            new_redir = (t_redir *)malloc(sizeof(t_redir));
            new_redir->filename = ft_strdup(tmp->next->value);
            new_redir->type = what_direction(tmp->value);
            new_redir->next = NULL;
            if (redir == NULL)
                redir = new_redir;
            else
            {
                tmp2 = redir;
                while (tmp2->next != NULL)
                    tmp2 = tmp2->next;
                tmp2->next = new_redir;
            }
        }
        tmp = tmp->next;
    }
    return redir;
}

void	list_commands(t_input *tok, t_command **cmd_list)
{
	t_command *new_cmd;
	t_command *tmp;

	while(tok != NULL)
    {
        new_cmd = (t_command *)malloc(sizeof(t_command));
        new_cmd->cmd = ft_strdup((tok)->value);
        new_cmd->args = put_the_args(tok, tok->value);
        new_cmd->inoutfile = check_derctions(tok , tok->value);
        new_cmd->next = NULL;
        if (*cmd_list == NULL)
            *cmd_list = new_cmd;
        else
        {
            tmp = *cmd_list;
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new_cmd;
        }
		while (tok && tok->type != PIPE)
			tok = tok->next;
		if (tok)
			tok = tok->next;
    }
}

void sigint_handler(int signal)
{
    if (signal == SIGINT)
    {
        write(1, "\n", 1);
        write(1, "minishell$ ", 11);
    }
    else if (signal == SIGQUIT)
        exit(1);
}

char **cpy_env(char **env)
{
    int i;
    char **p;

    i = 0;
    p = NULL;
    while (env[i])
        i++;
    p = malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (env[i])
    {
        p[i] = ft_strdup(env[i]);
        i++;
    }
    p[i] = NULL;
    return (p);
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	char	*line;
	t_input	*tok;
	list_env	*env_list;
	t_command	*cmd_list;

	env_list = NULL;
	cmd_list = NULL;
	tok = NULL;
	if (ac != 1)
		return (printf("Error: Too many arguments\n"), 1);
	ft_list_env(env, &env_list);
	while(1)
	{
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
		line = readline("minishell$ ");
		if (!line)
			return(printf("Exiting...\n"), 1);
		if (strlen(line) > 0)
		{
			add_history(line);

			tokenization(line, &tok);
			expand_variables(&tok, env_list);
			list_commands(tok, &cmd_list);
            parsing_tokns(tok, &cmd_list, env_list);

            int j = 1;
            t_command *cmd_list2 = cmd_list;
            t_redir  *redir =  cmd_list2->inoutfile;
            while (cmd_list2)
            {
                    redir =  cmd_list2->inoutfile;
                    printf("command %d:\n", j);
                    printf("cmd :%s\n", cmd_list2->cmd);
                    printf("args :");
                    for (size_t i = 0; cmd_list2->args[i]; i++)
                    {
                        printf("%s  ", cmd_list2->args[i]);
                    }
                    printf("\n");
                    while(redir)
                    {
                        printf("filename :%s   type:%d\n",  redir->filename, redir->type);
                        redir = redir->next;
                    }
                    cmd_list2 = cmd_list2->next;
                    j++;
            }

            exection(cmd_list, &env_list);
            // exit(1);
            cmd_list = NULL;
            tok = NULL;
		}
	}
    rl_clear_history();
}

