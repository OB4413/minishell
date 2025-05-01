/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokns.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:34:00 by obarais           #+#    #+#             */
/*   Updated: 2025/04/28 16:49:32 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void 	parsing_tokns(t_input *tok)
{
	t_input *tmp;
	int i = 0;

	tmp = tok;
	while (tmp)
	{
		if(tmp->type == HEREDOC)
			i++;
		else if (tmp->type == PIPE)
			i = 0;
		tmp = tmp->next;
	}
	if (i > 16)
	{
		printf("Error: Too many heredocs\n");
		exit(1);
	}
}
