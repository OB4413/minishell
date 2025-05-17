/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 09:24:57 by obarais           #+#    #+#             */
/*   Updated: 2025/05/08 15:32:23 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	*ft_malloc(size_t size, int type)
{
	static t_list	*head = NULL;
	t_list			*new_node;

	if (type == 0)
	{
		new_node = malloc(sizeof(t_list));
		if (!new_node)
			return (NULL);
		new_node->content = malloc(size);
		new_node->next = NULL;
		if (!head)
			head = new_node;
		else
		{
			new_node->next = head;
			head = new_node;
		}
		return (new_node->content);
	}
	else if (type == 1)
		ft_lstclear(&head, free);
	return (NULL);
}
