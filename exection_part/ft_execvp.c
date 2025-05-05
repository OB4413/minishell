/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 08:43:07 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/05 09:43:55 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int is_executable(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

char* search_path(const char *file)
{
    if(ft_strcmp(file,"./minishell") == 0)
        return("./minishell");
    int i = 0;
    char *path = getenv("PATH");
    if (!path)
        return NULL;
    char *path_copy = ft_strdup(path);
    char *full_path = NULL;
    char **dir = ft_split1(path_copy, ':');
    while(dir[i])
    {
        full_path = malloc(ft_strlen(dir[i]) + ft_strlen(file) + 2);
        full_path = ft_strjoin(dir[i], "/");
        full_path = ft_strjoin(full_path, file);
        if (is_executable(full_path))
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        i++;
    }
    free(path_copy);
    return NULL;
}

void change_table(char *new_str, char ***tab)
{
    int (i), (j);
    i = 0;
    j = -1;
    while ((*tab)[i])
        i++;
    char **tab2 = malloc((i + 2) * sizeof(char *));
    if (!tab2)
        return;
    tab2[0] = new_str;
    while(++j < i)
        tab2[j + 1] = (*tab)[j];
    tab2[i + 1] = NULL;
    *tab = tab2;
}

int execve_like_execvp(const char *file, char **argv, char **env)
{
    char *full_path;
    char *path;

    path = getenv("PATH");
    full_path = NULL;
    if (!path)
    {
        errno = ENOENT;
        return -1;
    }
    full_path = search_path(file);
    change_table(full_path, &argv);
    if (ft_strchr(file, '/'))
        return execve(full_path, argv, env);
    if (!full_path)
    {
        errno = ENOENT;
        return -1;
    }
    int result = execve(full_path, argv, env);
    free(full_path);
    return result;
}
