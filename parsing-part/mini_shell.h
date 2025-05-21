/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:56:27 by obarais           #+#    #+#             */
/*   Updated: 2025/05/21 17:14:36 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_input_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
}						t_input_type;

typedef struct s_input
{
	char				*value;
	t_input_type		type;
	struct s_input		*next;
}						t_input;

typedef struct s_list_env
{
	char				*key;
	char				*value;
	int					equal;
	struct s_list_env	*next;
}						t_list_env;

typedef struct s_redir
{
	char				*filename;
	int					type;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*inoutfile;
	char				*heredoc;
	struct s_command	*next;
}						t_command;

void					tokenization(char *line, t_input **tok);
void					expand_variables(t_input **tok, t_list_env *env, int i);
int						parsing_tokns(t_input *tok, t_command **cmd_list,
							t_list_env *env);
void					exection(struct s_command *cmd_list,
							t_list_env **env_list);
void					sigint_handler(int signal);
char					*get_value(char *str, t_list_env *env);
char					*ft_strjoin_c(char *s1, char c);
void					*ft_malloc(size_t size, int type);
char					*help_expand_variables(char *str, t_list_env *env,
							int i);
char					*get_next_line(int fd);
void					help_list_env1(t_list_env **new_env, int *c,
							t_list_env **env_list);
void					help_list_env(char **env, t_list_env **new_env,
							t_list_env **tmp, t_list_env **env_list);
void					ft_list_env(char **env, t_list_env **env_list);
char					**help_put_the_args(char **args, t_input **tmp, int *i,
							int *j);
char					**put_the_args(t_input *tok, char *cmd, char **args,
							t_input *tmp);
int						what_direction(char *str);
void					help_check_derction(t_input **tmp, t_redir **new_redir,
							t_redir **tmp2, t_redir **redir);
void					expand_heredoc(char **str, t_list_env *env, int i);
int						help_expand_heredoc(char **str, int *i, int *start);
char					*random_str(void);
void					help_move_quote(char **str, int *j, char **tmp);
char					*check_qout_closed(char *str);
char					*move_quote(char *str, int i, int j);
void					remove_quote(char **str, int i);
void					help_handel_heredoc(t_input *temp, t_list_env *env,
							t_command **hh, t_command **cmd_list);
void					run_children(char **tmp, t_list_env *env, t_input *tok,
							int fd);
int						help_run_children(char **tmp, t_list_env *env,
							char **str, t_input *tok);
int						help_handel_heredoc3(int *fd, t_command **hh,
							t_list_env *env, char *str);
int						help_handel_heredoc2(t_input *tok, t_command **hh,
							t_list_env *env, int fd);
int						help_handel_heredoc1(pid_t pid, int status,
							t_list_env *env, t_command **hh);
void					handler_sig_heredoc(int sig);
void					help_count_word(char *line, int *k, int *i, char *d);
void					skip_wait_space(char *line, int *i);
int						ft_count_word_min(char *line, int j, int k, char d);
char					*get_value(char *str, t_list_env *env);
void					help_split_to_tokens3(char *str, t_input **temp, int *i,
							int *start);
char					*help_split_to_tokens2(char *k, t_input **temp,
							t_input **temp1, char **temp2);
int						help_split_to_tokens1(char *str, t_input **temp, int *i,
							char *tokn);
char					*help_split_to_tokens(char h, t_input **temp,
							t_input **temp1, char **temp2);
char					*ft_strjoin_c(char *s1, char c);
int						help_split_to_tokens4(char *tokn, t_input **temp,
							char **temp2, t_input **temp1);
int						help_expand_variables1(int *i, char *str,
							t_list_env *env, char **tokn);
char					*ft_check_quote(char *str, t_list_env *env, char q,
							int i);
int						help_chek_quote(char *str, char **tokn, t_list_env *env,
							int *i);
int						help_chek_quote1(char *str, char **tokn,
							t_list_env *env, int *i);
char					*split_to_tokens(char *tokn, t_input **temp, char *str,
							char h);

#endif
