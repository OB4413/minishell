/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:23:38 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/09 10:37:55 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t len = 0;
    char *dup;

    if(!s)
        return NULL;
    while (len < n && s[len])
        len++;
    dup = ft_malloc((len + 1), 0);
    if (!dup)
	{
        return NULL;
	}
	len = 0;
    while (len < n && s[len])
	{
		dup[len] = s[len];
		len++;
	}
    dup[len] = '\0';
    return dup;
}
