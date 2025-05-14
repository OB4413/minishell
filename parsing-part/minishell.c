/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:56:34 by obarais           #+#    #+#             */
/*   Updated: 2025/05/14 17:17:05 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	ft_list_env(char **env, list_env **env_list)
{
	int i;
	int j;
    int c;
	list_env *new_env;
	list_env *tmp;

	i = 0;
	j = 0;
    new_env = ft_malloc(sizeof(list_env), 0);
    new_env->key = ft_strdup("$?");
    new_env->value = "0";
    new_env->next = NULL;
    *env_list = new_env;
	while (env[i])
    {
        j = 0;
        while (env[i][j] != '=')
            j++;
        new_env = ft_malloc(sizeof(list_env), 0);
        if (!new_env)
            return ;
        new_env->key = ft_substr(env[i], 0, j);
        new_env->value = ft_substr(env[i], j + 1, ft_strlen(env[i]) - j);
        // zedt had lIF condition besh ytincrementa shell level :)
        if(ft_strcmp(new_env->key, "SHLVL") == 0)
		{
			c = ft_atoi(new_env->value);
			c++;
            if (c > 999)
            {
                printf("warning: shell level (%d) too high, resetting to 1\n", c);
                c = 1;
            }
            else if(c < 0)
                c = 0;
            new_env->value = ft_strdup(ft_itoa(c));
		}
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

char ** put_the_args(t_input *tok, char *cmd)
{
    char **args;
    int i = 0;
    int j = 0;
    t_input *tmp2;
    t_input *tmp;

    tmp = tok;
    if (!cmd || !tok->value)
        return (NULL);
    while (ft_strcmp(tmp->value, cmd) != 0)
        tmp = tmp->next;
    tmp2 = tmp;
    while(tmp2 && tmp2->type != PIPE && tmp2->value)
    {
        if (tmp2->type == HEREDOC || tmp2->type == APPEND || tmp2->type == REDIRECT_IN || tmp2->type == REDIRECT_OUT)
        {
            if (!tmp2->next)
                break;
            tmp2 = tmp2->next;
            tmp2 = tmp2->next;
            continue;
        }
        i++;
        tmp2 = tmp2->next;
    }
    printf("%d\n", i);
    args = ft_malloc(sizeof(char *) * (i + 1), 0);
    if (args == NULL)
        return (NULL);
    while (tmp  && tmp->type != PIPE && tmp->value)
    {
        if (tmp->type == HEREDOC || tmp->type == APPEND || tmp->type == REDIRECT_IN || tmp->type == REDIRECT_OUT)
        {
            if (!tmp->next)
                break;
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
    if (ft_strcmp(str, "<") == 0)
        return 0;
    else if (ft_strcmp(str, ">") == 0)
        return 1;
    else if (ft_strcmp(str, ">>") == 0)
        return 2;
    else if (ft_strcmp(str, "<<") == 0)
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
    if(!cmd)
        return (NULL);
    while (ft_strcmp(tmp->value, cmd) != 0)
        tmp = tmp->next;
    while (tmp && tmp->type != PIPE)
    {
        if (tmp->type == HEREDOC || tmp->type == APPEND || tmp->type == REDIRECT_IN || tmp->type == REDIRECT_OUT)
        {
            new_redir = ft_malloc(sizeof(t_redir), 0);
            if (tmp->next)
                new_redir->filename = ft_strdup(tmp->next->value);
            else
                new_redir->filename = NULL;
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
        new_cmd = ft_malloc(sizeof(t_command), 0);
        new_cmd->args = put_the_args(tok, tok->value);
        new_cmd->heredoc = NULL;
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
    (void)signal;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

char **cpy_env(char **env)
{
    int i;
    char **p;

    i = 0;
    p = NULL;
    while (env[i])
        i++;
    p = ft_malloc(sizeof(char *) * (i + 1), 0);
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
		return (write(2, "Error: Too many arguments\n", 27), 127);
	ft_list_env(env, &env_list);
	while(1)
	{
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
        if (!isatty(STDIN_FILENO))
        {
            line = get_next_line(0);
            line = ft_strtrim(line, "\n");
        }
        else
		    line = readline("minishell$ ");
		if (!line)
        {
            printf("exit\n");
            break;
        }
		if (strlen(line) > 0)
		{
            add_history(line);

			tokenization(line, &tok);
			expand_variables(&tok, env_list);
			list_commands(tok, &cmd_list);
            if (parsing_tokns(tok, &cmd_list, env_list) == 1)
            {
                cmd_list = NULL;
                tok = NULL;
                continue;
            }




            int j = 1;
            t_command *cmd_list2 = cmd_list;
            t_redir  *redir =  cmd_list2->inoutfile;
            printf("%s\n", cmd_list2->heredoc);
            while (cmd_list2)
            {
                    redir =  cmd_list2->inoutfile;
                    printf("command %d:\n", j);
                    printf("args :");
                    if (cmd_list2->args)
                    {
                        for (size_t i = 0; cmd_list2->args[i]; i++)
                        {
                            printf("[%s]  ", cmd_list2->args[i]);
                        }
                    }
                    printf("\n");
                    while(redir)
                    {
                        printf("filename :[%s]   type:[%d]\n",  redir->filename, redir->type);
                        redir = redir->next;
                    }
                    cmd_list2 = cmd_list2->next;
                    j++;
            }
            exection(cmd_list, &env_list);
            cmd_list = NULL;
            tok = NULL;
		}
	}
    ft_malloc(1, 1);
    rl_clear_history();
    return(0);
}

