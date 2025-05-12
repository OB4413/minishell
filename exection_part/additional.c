/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 05:34:35 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/11 13:28:58 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

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

// void printed_min(int printed, char **min, int j, char **arr, int *min_index)
// {
// 	if (!printed)
// 	{
// 		if (!(*min) || ft_strcmp(arr[j], *min) < 0)
// 		{
// 			*min = arr[j];
// 			*min_index = j;
// 		}
// 	}
// }


// void	sorte_table(char **arr, int size)
// {
// 	char	(*min);

// 	int (printed[size]), (min_index), (i), (j);
// 	i = 0;
// 	while (i < size)
// 		printed[i++] = 0;
// 	i = -1;
// 	while (++i < size)
// 	{
// 		min = NULL;
// 		min_index = -1;
// 		j = -1;
// 		while (++j < size)
// 		{
// 			printed_min(printed[j], &min, j, arr, &min_index);
// 		}
// 		if (min)
// 		{
// 			printf("declare -x %s\n", min);
// 			printed[min_index] = 1;
// 		}
// 	}
// }

void	sorte_table(char **arr, int size)
{
	char	(*min);

	int (printed[size]), (min_index), (i), (j);
	i = 0;
	while (i < size)
		printed[i++] = 0;
	i = -1;
	while (++i < size)
	{
		min = NULL;
		min_index = -1;
		j = -1;
		while (++j < size)
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
