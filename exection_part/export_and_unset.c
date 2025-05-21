/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:20:21 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/20 22:12:41 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	remove_env_var(t_list_env **list, const char *name)
{
	t_list_env	*list1;
	t_list_env	*list2;

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

static char	*new_val(t_list_env *list2, const char *name_val, size_t name_len)
{
	char	*new_name;

	new_name = ft_strjoin(list2->value, ft_strdup(name_val + (name_len
					+ 1)));
	return (new_name);
}

static int	find_env_var(t_list_env **list, const char *name,
		const char *name_val)
{
	t_list_env	*list2;
	size_t		name_len;

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
			list2->value = new_val(list2, name_val, name_len);
			list2->equal = 1;
			return (1);
		}
		list2 = list2->next;
	}
	return (0);
}

void	set_env_var(t_list_env **list, const char *name_value)
{
	char	*name;
	char	*eq;

	t_list_env (*node);
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
