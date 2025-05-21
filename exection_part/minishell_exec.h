/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:23:34 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/21 15:01:38 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXEC_H
# define MINISHELL_EXEC_H

# include "../parsing-part/libft/libft.h"
# include "../parsing-part/mini_shell.h"
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <wait.h>

typedef struct s_env_data
{
	char	**arr;
	int		*printed;
}			t_env_data;

struct	s_command;
struct	s_list_env;
struct	s_redir;
void		ft_exit_status(int n);
int			ft_strcmp(const char *s1, const char *s2);
char		**ft_split1(char const *s, char c);
char		*ft_strdup(const char *s1);
char		*ft_strchr(const char *s, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlen(const char *s);
char		*ft_strrchr(const char *s, int c);
char		*ft_strndup(const char *s, size_t n);
void		ft_unset(char **args, char ***env, t_list_env **list);
void		ft_export(char **args, char **env, t_list_env **list);
char		*ft_strjoin(char const *s1, char const *s2);
int			execve_like_execvp(const char *file, char **argv, char **env,
				t_list_env **list_env);
void		exection(struct s_command *cmd_list, struct s_list_env **env_list);
char		**list_to_table(t_list_env *list);
char		*ft_strstr(char *str, char *to_find);
int			is_redirection(t_redir *redir, t_list_env **env_list, char *file);
int			heredoc_redirection(t_redir *redir, char *file,
				t_list_env **env_list);
void		execute_cmd(t_command *cmd_list, t_list_env **env_list);
void		sorte_table(char **arr, int size);
int			ft_lstsize2(t_list_env *lst);
void		execute_piped_commands(t_command *cmd, t_list_env **env_list);
char		**list_to_table_export(t_list_env *list);
t_list_env	*ft_getenv(t_list_env **env_list, char *str);
int			ft_contains(char *str);
int			remove_env_var(t_list_env **list, const char *name);
void		ft_lstadd_back2(t_list_env **lst, t_list_env *new);
void		set_env_var(t_list_env **list, const char *name_value);
t_list_env	*ft_lstlast2(t_list_env *lst);
int			get_flags(int type);
int			before_redir(t_command *cmd, t_list_env **env_list, char *file);
int			handle_output_redirection(t_redir *redir, t_list_env **env);
int			handle_input_redirection(t_redir *redir, t_list_env **env);
void		ft_echo(char **cmdlist, t_list_env **env_list);
char		*change_dir(t_list_env **env_list);
void		ft_cd(char **cmdlist, t_list_env **env_list);
void		ft_exit(char **args, t_list_env **list);
int			is_number(char *str);

#endif
