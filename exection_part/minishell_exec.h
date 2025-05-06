/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:23:34 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/06 15:32:17 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef MINISHELL_EXEC_H
# define MINISHELL_EXEC_H

#include <stdbool.h>
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include "../parsing-part/mini_shell.h"
#include "../parsing-part/libft/libft.h"
# include <wait.h>


struct  s_command;
struct  s_list_env;
struct  s_redir;
int	    ft_strcmp(const char *s1, const char *s2);
char	**ft_split1(char const *s, char c);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strrchr(const char *s, int c);
char    *ft_strndup(const char *s, size_t n);
void    ft_unset(char **args, char ***env, list_env **list);
void    ft_export(char **args, char **env, list_env **list);
char	*ft_strjoin(char const *s1, char const *s2);
int     execve_like_execvp(const char *file, char **argv, char **env);
void    exection(struct s_command *cmd_list, struct s_list_env **env_list);
char    **list_to_table(list_env *list);
char	*ft_strstr(char *str, char *to_find);
int     is_redirection(t_command *cmd, list_env **env_list, char ***env);
void    heredoc_redirection(struct s_command *cmd);
void    in_heredoc_redirs(struct s_command *cmd, list_env **env_list, char ***env);
void    execute_cmd(t_command *cmd_list, list_env **env_list, char ***env);
void    sorte_table(char **arr);
int	    ft_lstsize2(list_env *lst);
void    execute_piped_commands(t_command *cmd, list_env **env_list, char ***env);



#endif
