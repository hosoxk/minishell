/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 09:18:58 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/14 13:04:57 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	handle_operator(t_token_tree **tree, t_token *token_list)
{
	t_token	*operator_token;

	operator_token = find_operator(token_list, OR);
	if (!operator_token)
		operator_token = find_operator(token_list, AND);
	if (operator_token)
	{
		*tree = get_token_node(operator_token->type, NULL);
		if (!*tree)
		{
			g_exit_status = 1;
			return (true);
		}
		build_token_tree(&(*tree)->right, operator_token->next);
		build_token_tree(&(*tree)->left, split_before(token_list,
				operator_token));
		free_token(operator_token);
		return (true);
	}
	return (false);
}

void	free_token_tree(t_token_tree *tree)
{
	if (!tree)
		return ;
	free_token_tree(tree->right);
	free_token_tree(tree->left);
	free_token_list(&(tree->token_list));
	free(tree);
}

void	build_token_tree(t_token_tree **tree, t_token *token_list)
{
	if (!token_list)
	{
		*tree = NULL;
		return ;
	}
	if (handle_parentheses_tree(tree, token_list))
		return ;
	if (handle_operator(tree, token_list))
		return ;
	*tree = get_token_node(WORD, token_list);
}

void	execute_token_tree(t_token_tree *tree, char ***env, t_token_tree *root)
{
	if (!tree)
	{
		return ;
	}
	if (tree->type == WORD)
		execute_sub_commands(tree, env, root);
	else if (tree->type == AND)
	{
		execute_token_tree(tree->left, env, root);
		if (g_exit_status == 0)
			execute_token_tree(tree->right, env, root);
	}
	else if (tree->type == OR)
	{
		execute_token_tree(tree->left, env, root);
		if (g_exit_status != 0)
			execute_token_tree(tree->right, env, root);
	}
}
