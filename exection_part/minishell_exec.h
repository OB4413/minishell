/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:23:34 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/01 15:24:26 by ael-jama         ###   ########.fr       */
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


int	    ft_strcmp(const char *s1, const char *s2);
char	**ft_split1(char const *s, char c);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strrchr(const char *s, int c);
char    *ft_strndup(const char *s, size_t n);
void    ft_unset(char **args, char ***env);
void    ft_export(char **args, char ***env);
char	*ft_strjoin(char const *s1, char const *s2);
int     execve_like_execvp(const char *file, char **argv);
struct s_command;
struct s_list_env;
void    exection(struct s_command *cmd_list, struct s_list_env **env_list);
char **list_to_table(list_env *list);


#endif
