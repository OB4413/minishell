/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:56:34 by obarais           #+#    #+#             */
/*   Updated: 2025/05/22 15:03:31 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int		g_exit_sgnal;

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
	g_exit_sgnal = 130;
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
	g_exit_sgnal = 0;
	if (strlen(line) > 0)
	{
		add_history(line);
		tokenization(line, &tok);
		expand_variables(&tok, env_list, 0);
		list_commands(tok, &cmd_list);
		if (parsing_tokns(tok, &cmd_list, env_list) == 1)
		{
			cmd_list = NULL;
			tok = NULL;
			return ;
		}
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
		if (g_exit_sgnal == 130)
			env_list->value = ft_strdup("130");
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
