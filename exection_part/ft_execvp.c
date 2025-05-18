/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 08:43:07 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/17 17:06:14 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	is_executable(const char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

char	*search_path(const char *file)
{
	int		i;
	char	*path;
	char	*path_copy;
	char	*full_path;
	char	**dir;

	if (ft_strcmp(file, "./minishell") == 0)
		return ("./minishell");
	i = 0;
	path = getenv("PATH");
	if (!path)
		return (NULL);
	path_copy = ft_strdup(path);
	full_path = NULL;
	dir = ft_split1(path_copy, ':');
	while (dir[i])
	{
		full_path = ft_malloc((ft_strlen(dir[i]) + ft_strlen(file) + 2), 0);
		full_path = ft_strjoin(dir[i], "/");
		full_path = ft_strjoin(full_path, file);
		if (is_executable(full_path))
		{
			return (full_path);
		}
		i++;
	}
	return (NULL);
}

void	change_table(char *new_str, char ***tab)
{
	char	**tab2;

	int(i), (j);
	i = 0;
	j = -1;
	while ((*tab)[i])
		i++;
	tab2 = ft_malloc(((i + 2) * sizeof(char *)), 0);
	if (!tab2)
		return ;
	tab2[0] = new_str;
	while (++j < i)
		tab2[j + 1] = (*tab)[j + 1];
	tab2[i + 1] = NULL;
	*tab = tab2;
}

int	execve_like_execvp(const char *file, char **argv, char **env)
{
	char	*full_path;
	char	*path;
	int		result;

	path = getenv("PATH");
	full_path = NULL;
	if (!path)
	{
		errno = ENOENT;
		return (-1);
	}
	full_path = search_path(file);
	// change_table(full_path, &argv);
	if (ft_strchr(file, '/'))
		return (execve(full_path, argv, env));
	if (!full_path)
	{
		errno = ENOENT;
		return (-1);
	}
	result = execve(full_path, argv, env);
	return (result);
}
