/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:56:27 by obarais           #+#    #+#             */
/*   Updated: 2025/05/09 15:14:01 by obarais          ###   ########.fr       */
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
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
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
    int   type; // 0 input 1 output 2 APPAND 3 HEREDOC
    struct s_redir *next;
}					t_redir;

typedef struct s_command
{
    char    **args;
    t_redir *inoutfile;
	char	*heredoc;
    struct s_command *next;
}					t_command;

void	tokenization(char *line, t_input **tok);
void	expand_variables(t_input **tok, list_env *env);
void 	parsing_tokns(t_input *tok, t_command **cmd_list, list_env *env);
void    exection(struct s_command *cmd_list, list_env **env_list);
void	sigint_handler(int signal);
char	*get_value(char *str, list_env *env);
char *ft_strjoin_c(char *s1, char c);
char	**ft_split_7(char const *s);
void	*ft_malloc(size_t size, int type);
char *help_expand_variables(char *str, list_env *env);

#endif
