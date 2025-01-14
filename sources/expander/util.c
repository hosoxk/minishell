/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:07:36 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/14 13:25:38 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	swap(t_token *a, t_token *b)
{
	char			*tmp;
	t_token_type	tmp_type;

	tmp = a->value;
	a->value = b->value;
	b->value = tmp;
	tmp_type = a->type;
	a->type = b->type;
	b->type = tmp_type;
}

void	sort_linkt_list(t_token *list)
{
	t_token	*current;
	t_token	*next;

	current = list;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcasecmp(current->value, next->value) > 0)
			{
				swap(current, next);
			}
			next = next->next;
		}
		current = current->next;
	}
}
