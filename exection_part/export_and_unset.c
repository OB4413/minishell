/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:20:21 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/17 14:10:08 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int	remove_env_var(t_list_env **list, const char *name)
{
	t_list_env	(*list1), (*list2);
	list1 = *list;
	while (list1 && list1->next)
	{
		list2 = list1->next;
		if (ft_strcmp(list2->key, name) == 0)
		{
			list1->next = list2->next;
			return (0);
		}
		list1 = list1->next;
	}
	list2 = *list;
	return (0);
}

void	ft_unset(char **args, char ***env, t_list_env **list)
{
	int	i;
	t_list_env *tmp;

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

t_list_env	*ft_lstlast2(t_list_env *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back2(t_list_env **lst, t_list_env *new)
{
	t_list_env	*last;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast2(*lst);
	last->next = new;
}

static int	find_env_var(t_list_env **list, const char *name,
		const char *name_val)
{
	t_list_env	*list2;
	size_t		name_len;
	char		*new_val;

	if (!name)
		return (0);
	name_len = ft_strlen(name);
	list2 = *list;
	while (list2)
	{
		if (ft_strcmp(list2->key, name) == 0 && name_val[name_len] == '=')
		{
			list2->value = ft_strdup(name_val + (name_len + 1));
			return (1);
		}
		if (ft_strcmp(list2->key, ft_strndup(name, ft_strlen(name) - 1)) == 0
			&& name_val[name_len - 1] == '+' && name_val[name_len] == '=')
		{
			new_val = ft_strjoin(list2->value, ft_strdup(name_val + (name_len
							+ 1)));
			list2->value = new_val;
			list2->equal = 1;
			return (1);
		}
		list2 = list2->next;
	}
	return (0);
}

// Helper to add/replace an env var
static void	set_env_var(t_list_env **list, const char *name_value)
{
	t_list_env (*node);
	char (*name), (*eq);
	node = ft_malloc((sizeof(t_list_env)), 0);
	name = NULL;
	eq = ft_strchr(name_value, '=');
	if (eq)
		name = ft_strndup(name_value, eq - name_value);
	if (name && find_env_var(list, name, name_value))
		return ;
	if (!eq)
	{
		node->equal = 0;
		node->key = ft_strdup(name_value);
		node->value = NULL;
	}
	else
	{
		node->equal = 1;
		node->key = ft_strndup(name_value, eq - name_value);
		node->value = ft_strdup(eq + 1);
	}
	node->next = NULL;
	ft_lstadd_back2(list, node);
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

int	ft_contains(char *str)
{
	int	i;

	char(*name), (*eq);
	i = 32;
	name = NULL;
	eq = ft_strchr(str, '=');
	if (eq)
		name = ft_strndup(str, eq - str);
	else
		name = ft_strdup(str);
	if (ft_isdigit(str[0]) == 1)
		return (-1);
	while (i < 127)
	{
		if (ft_isalpha(i) == 0 && ft_isdigit(i) == 0 && i != 95)
		{
			if (ft_strchr(name, i) != NULL)
			{
				return (-1);
			}
		}
		i++;
	}
	return (0);
}

// Main export function
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
		if (ft_contains(args[i]) == -1 || ft_strcmp(args[i], "=") == 0)
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

char	**list_to_table_export(t_list_env *list)
{
	int		i;
	char	**env2;

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
