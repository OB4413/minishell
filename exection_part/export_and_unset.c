/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:20:21 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/01 17:13:36 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static void remove_env_var(char ***env, const char *name)
{
    char **new_env;
    int (i), (j), (name_len), (count), (found_index);
    name_len = strlen(name);
    count = 0;
    found_index = -1;
    while ((*env)[count])
    {
        if (strncmp((*env)[count], name, name_len) == 0 && 
        (*env)[count][name_len] == '=')
            found_index = count;
        count++;
    }
    if (found_index == -1)
        return;
    new_env = malloc(count * sizeof(char *));
    if (!new_env)
        return;
    i = 0;
    j = 0;
    while (i < count)
    {
        if (i != found_index)
            new_env[j++] = (*env)[i];
        else
            free((*env)[i]);
        i++;
    }
    new_env[j] = NULL;
    free(*env);
    *env = new_env;
}

void ft_unset(char **args, char ***env)
{
    if (!args[0])
    {
        write(2, "unset: not enough arguments\n", 27);
        return;
    }
    for (int i = 0; args[i]; i++)
    {
        if (strchr(args[i], '='))
        {
            printf("unset: `%s': not a valid identifier\n", args[i]);
            continue;
        }
        remove_env_var(env, args[i]);
    }
}


static int find_env_var(char ***env, const char *name, const char *name_val)
{
    size_t name_len = ft_strlen(name);
    char *new_val;
    for (int i = 0; (*env)[i]; i++)
    {
        if (ft_strncmp((*env)[i], name, name_len - 1) == 0 && (*env)[i][name_len] == '+')
        {
            new_val = ft_strdup(name_val + (name_len + 2));
            free((*env)[i]);
            (*env)[i] = ft_strjoin(*(env)[i], new_val);
            return 1;
        }
        if (ft_strncmp((*env)[i], name, name_len - 1) == 0 && (*env)[i][name_len] == '=')
        {
            free((*env)[i]);
            (*env)[i] = ft_strdup(name_val);
            return 1;
        }
    }
    return 0;
}

// Helper to add/replace an env var
static void set_env_var(char ***env, const char *name_value)
{
    char (**new_env), (*name), (*eq);
    int (count), (i);
    count = 0;
    eq = ft_strchr(name_value, '=');
    if (eq)
        name = ft_strndup(name_value, eq - name_value);
    int existing = find_env_var(env, name, name_value);
    if (existing)
        return;
    while ((*env)[count])
        count++;
    new_env = malloc((count + 2) * sizeof(char *));
    if (!new_env)
        return;
    i = -1;
    while(++i < count)
        new_env[i] = ft_strdup((*env)[i]);
    new_env[count] = ft_strdup(name_value);
    new_env[count + 1] = NULL;
    // free(*env);
    *env = new_env;
    
}

void add_to_env_var(char ***env, const char *name_value)
{
    char (**new_env), (*name), (*eq);
    int (count), (i);
    count = 0;
    eq = ft_strchr(name_value, '+');
    if (eq)
        name = ft_strndup(name_value, eq - name_value);
    int existing = find_env_var(env, name, name_value);
    if (existing)
        return;
    while ((*env)[count])
        count++;
    new_env = malloc((count + 2) * sizeof(char *));
    if (!new_env)
        return;
    i = -1;
    while(++i < count)
        new_env[i] = ft_strdup((*env)[i]);
    new_env[count] = ft_strdup(name_value);
    new_env[count + 1] = NULL;
    // free(*env);
    *env = new_env;    
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

// Main export function
void ft_export(char **args, char ***env)
{
    if (!args[0])
    {
        for (int i = 0; (*env)[i]; i++)
        {
            write(1, (*env)[i], strlen((*env)[i]));
            write(1, "\n", 1);
        }
        return;
    }
    
    for (int i = 0; args[i]; i++)
    {
        if (ft_strstr(args[i], "+=") && strchr(args[i], ' ') == NULL)
            add_to_env_var(env, args[i]);
        if (strchr(args[i], '=') && strchr(args[i], ' ') == NULL)
            set_env_var(env, args[i]);
        else
            printf("export: `%s': not a valid identifier\n", args[i]);
    }
}

int	ft_lstsize2(list_env *lst)
{
	size_t	i;
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

char **list_to_table(list_env *list)
{
    int size = ft_lstsize2(list);
    int i;
    char **env2;
    env2 = malloc(size * sizeof(char *));
    i = 0;
    while(list)
    {
        
        env2[i] = ft_strjoin(ft_strjoin(list->key, "="), list->value);
        i++;
        list = list->next;
    }
    env2[i] = NULL;
    return env2;
}