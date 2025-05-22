/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:01:28 by obarais           #+#    #+#             */
/*   Updated: 2025/05/22 16:08:53 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*split_to_tokens(char *tokn, t_input **temp, char *str, char h)
{
	t_input	*temp1;
	char	*temp2;
	int		start;
	int		i;

	if (!str)
		return (tokn);
	str = ft_strjoin(tokn, str);
	i = help_split_to_tokens4(tokn, temp, &temp2, &temp1);
	while (str[i])
	{
		while (!tokn && str[i] && str[i] <= 32)
			i++;
		start = i;
		if (help_split_to_tokens1(str, temp, &i, tokn) == 1)
			i--;
		else
			help_split_to_tokens3(str, temp, &i, &start);
		while (str[i] && str[i] <= 32)
			i++;
	}
	if (str[i - 1] <= 32)
		return (help_split_to_tokens(h, temp, &temp1, &temp2));
	return (help_split_to_tokens2(NULL, temp, &temp1, &temp2));
}

int	help_chek_quote1(char *str, char **tokn, t_list_env *env, int *i)
{
	int	start;

	if (str[*i] == '$' && (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'
			|| str[*i + 1] == '?'))
	{
		(*i)++;
		if (str[*i] >= 48 && str[*i] <= 57 && (*i)++)
			return (1);
		start = *i;
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'
				|| str[*i] == '?'))
		{
			(*i)++;
			if (str[*i - 1] == '?' && start--)
				break ;
		}
		*tokn = ft_strjoin(*tokn, get_value(ft_substr(str, start, *i - start),
					env));
		if (str[*i] && str[*i] == '"')
			(*i)++;
	}
	else if (str[*i])
	{
		*tokn = ft_strjoin_c(*tokn, str[*i]);
		(*i)++;
	}
	return (0);
}

int	help_chek_quote(char *str, char **tokn, t_list_env *env, int *i)
{
	if (str[*i] == '"')
	{
		(*i)++;
		while (str[*i] && str[*i] != '"')
		{
			if (help_chek_quote1(str, tokn, env, i) == 1)
				return (0);
		}
	}
	else if (str[*i] && str[*i] == '\'')
	{
		(*i)++;
		while (str[*i] && str[*i] != '\'')
		{
			*tokn = ft_strjoin_c(*tokn, str[*i]);
			(*i)++;
		}
		(*i)++;
	}
	else
	{
		*tokn = ft_strjoin_c(*tokn, str[*i]);
		(*i)++;
	}
	return (0);
}

char	*ft_check_quote(char *str, t_list_env *env, char q, int i)
{
	char	*tokn;

	tokn = NULL;
	while (str[i] && str[ft_strlen(str) - 1] == q && str[1] != '\0')
		help_chek_quote(str, &tokn, env, &i);
	if (str[ft_strlen(str) - 1] != q || str[1] == '\0')
		write(2, "Error: Unmatched quotes\n", 25);
	return (tokn);
}

int	help_expand_variables1(int *i, char *str, t_list_env *env, char **tokn)
{
	int	start;

	(*i)++;
	if (str[*i] >= 48 && str[*i] <= 57)
	{
		(*i)++;
		return (1);
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	*tokn = ft_strjoin(*tokn, get_value(ft_substr(str, start, *i - start),
				env));
	return (0);
}
