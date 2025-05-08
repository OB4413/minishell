/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 05:34:35 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/06 17:50:48 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}


void sorte_table(char **arr)
{
	int size = 0;
	while (arr[size])
		size++;

	int printed[size];
	for (int i = 0; i < size; i++)
		printed[i] = 0;

	for (int i = 0; i < size; i++)
	{
		char *min = NULL;
		int min_index = -1;
		for (int j = 0; j < size; j++)
		{
			if (!printed[j])
			{
				if (!min || ft_strcmp(arr[j], min) < 0)
				{
					min = arr[j];
					min_index = j;
				}
			}
		}
		if (min)
		{
			printf("declare -x %s\n", min);
			printed[min_index] = 1;
		}
	}
}