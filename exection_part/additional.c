/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 05:34:35 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/20 22:00:57 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1 && !s2)
		return (0);
	if (s1 == NULL || s2 == NULL)
		return (-1);
	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	sorte_table(char **arr, int size)
{
	char	*min;

	int (min_index), (i), (j), (printed[size]);
	i = 0;
	while (i < size)
		printed[i++] = 0;
	i = 0;
	while (i++ < size)
	{
		min = NULL;
		min_index = -1;
		j = 0;
		while (j < size)
		{
			if (!printed[j] && (!min || ft_strcmp(arr[j], min) < 0))
			{
				min = arr[j];
				min_index = j;
			}
			j++;
		}
		if (min)
			printf("declare -x %s\n", min);
		printed[min_index] = 1;
	}
}

int	ft_contains(char *str)
{
	int		i;
	char	*name;
	char	*eq;

	i = 31;
	eq = ft_strchr(str, '=');
	if (eq && ft_strcmp(eq + ft_strlen(eq) - 2, "+"))
		name = ft_strndup(str, eq - str - 1);
	else if (eq)
		name = ft_strndup(str, eq - str);
	else
		name = ft_strdup(str);
	if (ft_isdigit(str[0]) == 1)
		return (-1);
	while (++i < 127)
	{
		if (ft_isalpha(i) == 0 && ft_isdigit(i) == 0 && i != 95)
		{
			if (ft_strchr(name, i) != NULL)
				return (-1);
		}
	}
	return (0);
}

char	**list_to_table(t_list_env *list)
{
	int		i;
	char	**env2;

	i = ft_lstsize2(list);
	env2 = ft_malloc(((i + 1) * sizeof(char *)), 0);
	i = 0;
	list = list->next;
	while (list)
	{
		if (list->equal == 1 && list->value)
			env2[i] = ft_strjoin(ft_strjoin(list->key, "="), list->value);
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

void	ft_unset(char **args, char ***env, t_list_env **list)
{
	int			i;
	t_list_env	*tmp;

	(void)env;
	tmp = *list;
	if (!args[1])
	{
		(*list)->value = ft_strdup("0");
		return ;
	}
	i = 1;
	tmp = tmp->next;
	while (args[i])
	{
		(*list)->value = ft_strdup(ft_itoa(remove_env_var(&tmp, args[i])));
		i++;
	}
}
