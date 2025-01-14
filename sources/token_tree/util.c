/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 09:36:02 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/14 09:36:02 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tree	*get_token_node(t_token_type type, t_token *token_list)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->token_list = token_list;
	node->type = type;
	return (node);
}

t_token	*find_operator(t_token *token, t_token_type type)
{
	int	depth;

	depth = 0;
	while (token)
	{
		if (token->type == PARENTHESES_OPEN)
			depth++;
		if (token->type == PARENTHESES_CLOSE)
			depth--;
		if ((depth == 0 || type == PARENTHESES_OPEN) && token->type == type)
			return (token);
		token = token->next;
	}
	return (NULL);
}

t_token	*split_before(t_token *list, t_token *target)
{
	t_token	*current;

	current = list;
	while (current && current->next != target)
		current = current->next;
	if (current)
		current->next = NULL;
	return (list);
}

t_token	*find_last_operator(t_token *token_list)
{
	t_token	*last_op;
	t_token	*current;

	last_op = NULL;
	current = token_list;
	while (current)
	{
		if (current->type == AND || current->type == OR)
			last_op = current;
		current = current->next;
	}
	return (last_op);
}
