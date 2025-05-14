/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:20:21 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/14 14:55:04 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static void	remove_env_var(list_env **list, const char *name)
{
	list_env	(*list1), (*list2);
	list1 = *list;
	while (list1 && list1->next)
	{
		list2 = list1->next;
		if (ft_strcmp(list2->key, name) == 0)
		{
			list1->next = list2->next;
		}
		list1 = list1->next;
	}
	list2 = *list;
	while (list2)
	{
		printf("%s=%s\n", list2->key, list2->value);
		list2 = list2->next;
	}
}

void	ft_unset(char **args, char ***env, list_env **list)
{
	int	i;

	(void)env;
	if (!args[1])
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	i = 1;
	(*list) = (*list)->next;
	while (args[i])
	{
		if (strchr(args[i], '='))
		{
			printf("unset: `%s': not a valid identifier\n", args[i]);
			continue ;
		}
		remove_env_var(list, args[i]);
		i++;
	}
}

list_env	*ft_lstlast2(list_env *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back2(list_env **lst, list_env *new)
{
	list_env	*last;

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

static int	find_env_var(list_env **list, const char *name,
		const char *name_val)
{
	list_env	*list2;
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
static void	set_env_var(list_env **list, const char *name_value)
{
	list_env (*node);
	char (*name), (*eq);
	node = ft_malloc((sizeof(list_env)), 0);
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

int ft_contains(char *str)
{
	int i;
	char (*name), (*eq);

	i = 32;
	name = NULL;
	eq = ft_strchr(str, '=');
	if (eq)
		name = ft_strndup(str, eq - str);
	else
		name = ft_strdup(str);
	if(ft_isdigit(str[0]) == 1)
		return (-1);
	while(i < 127)
	{
		if(ft_isalpha(i) == 0 && ft_isdigit(i) == 0 && i != 95)
		{
			if(ft_strchr(name, i) != NULL){
				return (-1);}
		}
		i++;
	}
	return (0);
}

// Main export function
void	ft_export(char **args, char **env, list_env **list)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	if (!args[1])
	{
		sorte_table(env, i);
		(*list)->value = 0;
		return ;
	}
	i = 1;
	while (args[i])
	{
		if(ft_contains(args[i]) == -1)
		{
			write(2, "export : not a valid identifier\n", 31);
			// continue;
		}
		else
			set_env_var(list, args[i]);
		i++;
	}
	(*list)->value = 0;
}

int	ft_lstsize2(list_env *lst)
{
	size_t		i;
	list_env	*ptr;

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

char	**list_to_table(list_env *list)
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

char	**list_to_table_export(list_env *list)
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
