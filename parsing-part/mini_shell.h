/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:56:27 by obarais           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/04 16:34:10 by obarais          ###   ########.fr       */
=======
/*   Updated: 2025/05/03 10:59:36 by ael-jama         ###   ########.fr       */
>>>>>>> 6b671d2ad122ee514afe3d76cdd98461d5ee2f56
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINI_SHELL_H
# define MINI_SHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>

typedef enum e_input_type
{
	WORD,
	PIPE,
	REDIRECT_IN, // 2
	REDIRECT_OUT, // 3
	APPEND, // 4
	HEREDOC, // 5
}					t_input_type;

typedef struct s_input
{
	char			*value;
	t_input_type	type;
	struct s_input	*next;
}					t_input;

typedef struct s_list_env
{
	char			*key;
	char			*value;
	int				equal;
	struct s_list_env	*next;
}					list_env;

typedef struct s_redir
{
    char *filename;
    int   type;
    struct s_redir *next;
}					t_redir;

typedef struct s_command
{
	char    *cmd;
    char    **args;
    t_redir *inoutfile;
    struct s_command *next;
}					t_command;

void	tokenization(char *line, t_input **tok);
void	expand_variables(t_input **tok, list_env *env);
void 	parsing_tokns(t_input *tok);
void    exection(struct s_command *cmd_list, list_env **env_list);
void	sigint_handler(int signal);

#endif
