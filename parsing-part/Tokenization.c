/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:38:49 by obarais           #+#    #+#             */
/*   Updated: 2025/05/16 20:09:37 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void    help_count_word(char *line, int *k , int *i, char *d)
{
	(*i)++;
	if (line[*i] && line[*i] == *d)
	{
        *d = '\0';
		*k = 0;
		(*i)++;
	}
}

int ft_count_word(char *line, int j, int k, char d)
{
	int i;

	i = 0;
	while (line[i] <= 32 && line[i] != '\0')
		i++;
	while (line[i] != '\0')
	{
		if ((line[i] == '"' || line[i] == '\'') && k == 0)
		{
			if((i == 0 || line[i - 1] <= 32 || line[i - 1] == '|' || ft_strrchr("><", line[i - 1])))
				j++;
			d = line[i];
			k = 1;
		}
		if ((line[i] == '|' || ft_strchr("><", line[i]))&& k == 0)
		{
			if (line[i] == line[i + 1])
				i++;
			j++;
		}
		else if (line[i] > 32 && k == 0 && (i == 0 || line[i - 1] <= 32 || line[i - 1] == '|' || ft_strrchr("><", line[i - 1])))
			j++;
        help_count_word(line, &k, &i, &d);
	}
	return (j);
}

char    *help_alloc_and_cpy(char *s, int *i, int start)
{
    if (ft_strrchr("><", s[*i]) || s[*i] == '|')
    {
        if (s[*i] == '|')
            return ((*i)++,"|");
        if (ft_strrchr("><", s[*i]) && s[*i] == s[*i + 1])
            (*i)++;
        (*i)++;
        return (ft_substr(s, start, *i - start));
    }
    return (NULL);
}

char *ft_alloc_and_cpy(char *s, int *i, int start)
{
    char q;
    char *word;

    while (s[*i])
    {
        word = help_alloc_and_cpy(s, i, start);
        if (word)
            return (word);
        if (s[*i] == '"' || s[*i] == '\'')
        {
            q = s[*i];
            (*i)++;
            while (s[*i] && s[*i] != q)
                (*i)++;
            if (!s[*i])
                return (ft_substr(s, start, *i - start));
        }
        if (s[*i + 1] <= 32 || ft_strrchr("><", s[*i + 1]) || s[*i + 1] == '|')
        {
            (*i)++;
            return (ft_substr(s, start, *i - start));
        }
        (*i)++;
    }
    return (ft_substr(s, start, *i - start));
}

char **split_line(char *line)
{
    int i = 0;
    int j = 0;
	int words;
    char    **array;

	words = ft_count_word(line, 0, 0, '\0');
    array = ft_malloc((words + 1) * sizeof(char *), 0);
	if (array == NULL)
    return (NULL);
	while (line[i] && j < words)
	{
        while (line[i] && line[i] <= 32)
        i++;
		array[j] = ft_alloc_and_cpy(line, &i, i);
		if (!array[j])
			return (NULL);
		j++;
	}
	array[j] = NULL;
	return (array);
}

void    chosse_type(t_input **new)
{
    if ((*new)->value[0] == '|')
        (*new)->type = PIPE;
    else if ((*new)->value[0] == '<' && (*new)->value[1] == '<')
        (*new)->type = HEREDOC;
    else if ((*new)->value[0] == '<')
        (*new)->type = REDIRECT_IN;
    else if ((*new)->value[0] == '>' && (*new)->value[1] != '>')
        (*new)->type = APPEND;
    else if ((*new)->value[0] == '>')
        (*new)->type = REDIRECT_OUT;
    else
        (*new)->type = WORD;
}

void	tokenization(char *line, t_input **tok)
{
    char **p;
    int    i = 0;
    t_input *new;
    t_input *temp;

	p = split_line(line);

    while (p[i] != NULL)
    {
        new = ft_malloc(sizeof(t_input), 0);
        new->value = ft_strdup(p[i]);
        chosse_type(&new);
        new->next = NULL;
        if (*tok == NULL)
            *tok = new;
        else
        {
            temp = *tok;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = new;
        }
        i++;
    }
}
