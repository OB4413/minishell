/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:56:34 by obarais           #+#    #+#             */
/*   Updated: 2025/05/17 17:20:51 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	help_list_env1(t_list_env **new_env, int *c, t_list_env **env_list)
{
	if (ft_strcmp((*new_env)->key, "SHLVL") == 0)
	{
		*c = ft_atoi((*new_env)->value);
		(*c)++;
		if (*c > 999)
		{
			printf("warning: shell level (%d) too high, resetting to 1\n", *c);
			*c = 1;
		}
		else if (*c < 0)
			*c = 0;
		(*new_env)->value = ft_strdup(ft_itoa(*c));
	}
	(*new_env)->equal = 1;
	(*new_env)->next = NULL;
	if (*env_list == NULL)
		*env_list = *new_env;
}

void	help_list_env(char **env, t_list_env **new_env, t_list_env **tmp,
		t_list_env **env_list)
{
	int (i), (j), (c);
	i = 0;
	j = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != '=')
			j++;
		*new_env = ft_malloc(sizeof(t_list_env), 0);
		if (!(*new_env))
			return ;
		(*new_env)->key = ft_substr(env[i], 0, j);
		(*new_env)->value = ft_substr(env[i], j + 1, ft_strlen(env[i]) - j);
		help_list_env1(new_env, &c, env_list);
		if (*env_list)
		{
			*tmp = *env_list;
			while ((*tmp)->next != NULL)
				*tmp = (*tmp)->next;
			(*tmp)->next = *new_env;
		}
		i++;
	}
}

void	ft_list_env(char **env, t_list_env **env_list)
{
	t_list_env	*new_env;
	t_list_env	*tmp;

	new_env = ft_malloc(sizeof(t_list_env), 0);
	new_env->key = ft_strdup("$?");
	new_env->value = "0";
	new_env->next = NULL;
	*env_list = new_env;
	help_list_env(env, &new_env, &tmp, env_list);
}

char	**help_put_the_args(char **args, t_input **tmp, int *i, int *j)
{
	args = ft_malloc(sizeof(char *) * (*i + 1), 0);
	if (args == NULL)
		return (NULL);
	while (*tmp && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == HEREDOC || (*tmp)->type == APPEND
			|| (*tmp)->type == REDIRECT_IN || (*tmp)->type == REDIRECT_OUT)
		{
			if (!(*tmp)->next)
				break ;
			*tmp = (*tmp)->next->next;
			continue ;
		}
		args[*j] = ft_strdup((*tmp)->value);
		(*j)++;
		*tmp = (*tmp)->next;
	}
	args[*j] = NULL;
	return (args);
}

char	**put_the_args(t_input *tok, char *cmd, char **args, t_input *tmp)
{
	int (i), (j);
	t_input (*tmp2);
	i = 0;
	j = 0;
	tmp = tok;
	if (!cmd || !tok->value)
		return (NULL);
	while (ft_strcmp(tmp->value, cmd) != 0)
		tmp = tmp->next;
	tmp2 = tmp;
	while (tmp2 && tmp2->type != PIPE)
	{
		if (tmp2->type == HEREDOC || tmp2->type == APPEND
			|| tmp2->type == REDIRECT_IN || tmp2->type == REDIRECT_OUT)
		{
			if (!tmp2->next)
				break ;
			tmp2 = tmp2->next->next;
			continue ;
		}
		i++;
		tmp2 = tmp2->next;
	}
	return (help_put_the_args(args, &tmp, &i, &j));
}

int	what_direction(char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (0);
	else if (ft_strcmp(str, ">") == 0)
		return (1);
	else if (ft_strcmp(str, ">>") == 0)
		return (2);
	else if (ft_strcmp(str, "<<") == 0)
		return (3);
	return (-1);
}

void	help_check_derction(t_input **tmp, t_redir **new_redir, t_redir **tmp2,
		t_redir **redir)
{
	while (*tmp && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == HEREDOC || (*tmp)->type == APPEND
			|| (*tmp)->type == REDIRECT_IN || (*tmp)->type == REDIRECT_OUT)
		{
			*new_redir = ft_malloc(sizeof(t_redir), 0);
			if ((*tmp)->next)
				(*new_redir)->filename = ft_strdup((*tmp)->next->value);
			else
				(*new_redir)->filename = NULL;
			(*new_redir)->type = what_direction((*tmp)->value);
			(*new_redir)->next = NULL;
			if (*redir == NULL)
				*redir = *new_redir;
			else
			{
				*tmp2 = *redir;
				while ((*tmp2)->next != NULL)
					*tmp2 = (*tmp2)->next;
				(*tmp2)->next = *new_redir;
			}
		}
		*tmp = (*tmp)->next;
	}
}

t_redir	*check_derctions(t_input *tok, char *cmd)
{
	t_redir	*redir;
	t_input	*tmp;
	t_redir	*new_redir;
	t_redir	*tmp2;

	redir = NULL;
	tmp = tok;
	if (!cmd)
		return (NULL);
	while (ft_strcmp(tmp->value, cmd) != 0)
		tmp = tmp->next;
	help_check_derction(&tmp, &new_redir, &tmp2, &redir);
	return (redir);
}

void	list_commands(t_input *tok, t_command **cmd_list)
{
	t_command	*new_cmd;
	t_command	*tmp;

	while (tok != NULL)
	{
		new_cmd = ft_malloc(sizeof(t_command), 0);
		new_cmd->args = put_the_args(tok, tok->value, NULL, NULL);
		new_cmd->heredoc = NULL;
		new_cmd->inoutfile = check_derctions(tok, tok->value);
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

void	sigint_handler(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	help_main(char *line, t_list_env **invarmant)
{
	t_input		*tok;
	t_command	*cmd_list;
	t_list_env	*env_list;

	tok = NULL;
	cmd_list = NULL;
	env_list = *invarmant;
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
			return ;
		}
		if (!cmd_list->args)
			printf("fdfgsdg\n");
		exection(cmd_list, &env_list);
		cmd_list = NULL;
		tok = NULL;
	}
}

int	main(int ac, char **av, char **env)
{
	char		*line;
	t_list_env	*env_list;

	(void)av;
	env_list = NULL;
	if (ac != 1)
		return (write(2, "Error: Too many arguments\n", 27), 127);
	ft_list_env(env, &env_list);
	while (1)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		help_main(line, &env_list);
	}
	ft_malloc(1, 1);
	rl_clear_history();
	return (0);
}
