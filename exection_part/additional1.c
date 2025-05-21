/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:23:38 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/20 22:01:56 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*dup;

	len = 0;
	if (!s)
		return (NULL);
	while (len < n && s[len])
		len++;
	dup = ft_malloc((len + 1), 0);
	if (!dup)
	{
		return (NULL);
	}
	len = 0;
	while (len < n && s[len])
	{
		dup[len] = s[len];
		len++;
	}
	dup[len] = '\0';
	return (dup);
}

void	ft_export(char **args, char **env, t_list_env **list)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	if (!args[1])
	{
		sorte_table(env, i);
		(*list)->value = ft_strdup("0");
		return ;
	}
	i = 1;
	while (args[i])
	{
		if (ft_contains(args[i]) == -1)
		{
			write(2, " not a valid identifier\n", 25);
			(*list)->value = ft_strdup("1");
			return ;
		}
		else
			set_env_var(list, args[i]);
		i++;
	}
	(*list)->value = ft_strdup("0");
}

char	**list_to_table_export(t_list_env *list)
{
	int		i;
	char	**env2;

	list = list->next;
	i = ft_lstsize2(list);
	env2 = ft_malloc(((i + 1) * sizeof(char *)), 0);
	i = 0;
	while (list)
	{
		if (list->equal == 1 && list->value)
			env2[i] = ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin(list->key,
								"="), "\""), list->value), "\"");
		else if (list->equal == 1 && !list->value)
			env2[i] = ft_strjoin(list->key, "=");
		else
			env2[i] = list->key;
		i++;
		list = list->next;
	}
	env2[i] = NULL;
	return (env2);
}

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	if (to_find[0] == '\0')
		return (str);
	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (to_find[j] == str[i + j] && to_find[j] != '\0')
		{
			j++;
		}
		if (to_find[j] == '\0')
			return (str + i);
		i++;
	}
	return (0);
}

int	ft_lstsize2(t_list_env *lst)
{
	size_t		i;
	t_list_env	*ptr;

	if (lst == NULL)
		return (0);
	i = 0;
	ptr = lst;
	while (ptr != NULL)
	{
		i++;
		ptr = ptr->next;
	}
	return (i);
}
