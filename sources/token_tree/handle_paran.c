/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_paran.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 09:37:18 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/14 09:37:18 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*find_closing_parenthesis(t_token *open_paren)
{
	int			depth;
	t_token		*current;

	depth = 1;
	current = open_paren->next;
	while (current && depth > 0)
	{
		if (current->type == PARENTHESES_OPEN)
			depth++;
		else if (current->type == PARENTHESES_CLOSE)
			depth--;
		if (depth == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static void	insort_tree_after(t_token_tree **tree, t_token *after)
{
	t_token	*first_op;
	t_token_tree	*operator_node;

	if (!after)
		return ;
	first_op = find_operator(after, AND);
	if (!first_op)
		first_op = find_operator(after, OR);
	if (first_op)
	{
		operator_node = get_token_node(first_op->type, NULL);
		operator_node->left = *tree;
		build_token_tree(&operator_node->right, after);
		*tree = operator_node;
	}
}

static void	insort_tree_befor(t_token_tree **tree, t_token *before, t_token_tree *subtree)
{
	t_token	*last_op;
	t_token_tree	*operator_node;

	if (!before)
	{
		*tree = subtree;
		return ;
	}
	last_op = find_last_operator(before);
	if (last_op)
	{
		operator_node = get_token_node(last_op->type, NULL);
		build_token_tree(&operator_node->left, before);
		operator_node->right = subtree;
		*tree = operator_node;
	}
	else
		*tree = subtree;
}

static bool	divade_token_list(t_token *token_list, t_token **before,
		t_token **inside, t_token **after)
{
	t_token	*open_paren;
	t_token	*close_paren;

	open_paren = find_operator(token_list, PARENTHESES_OPEN);
	if (!open_paren)
		return (false);
	close_paren = find_closing_parenthesis(open_paren);
	*after = close_paren->next;
	*inside = open_paren->next;
	*before = NULL;
	if (open_paren != token_list)
		*before = split_before(token_list, open_paren);
	*inside = split_before(*inside, close_paren);
	return (true);
}

bool	handle_parentheses_tree(t_token_tree **tree, t_token *token_list)
{
	t_token_tree	*subtree;
	t_token	*before;
	t_token	*inside;
	t_token	*after;

	if (!divade_token_list(token_list, &before, &inside, &after))
		return (false);
	subtree = NULL;
	build_token_tree(&subtree, inside);
	insort_tree_befor(tree, before, subtree);
	insort_tree_after(tree, after);
	return (true);
}
