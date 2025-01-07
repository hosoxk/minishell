/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:12:50 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/06 14:45:40 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token_list(t_token **token_list)
{
	t_token	*tmp;

	if (!token_list || !*token_list)
		return ;
	while (*token_list)
	{
		tmp = (*token_list)->next;
		if ((*token_list)->value)
			free((*token_list)->value);
		free(*token_list);
		*token_list = tmp;
	}
	*token_list = NULL;
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->args)
	{
		int	i;

		i = 0;
		while (node->args[i])
			free(node->args[i++]);
		free(node->args);
	}
	if (node->file)
		free(node->file);
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	free_program(t_token *token_list, t_ast *ast_root)
{
	free_token_list(&token_list); // al gedaan voor execute
	// while (ast_root)
	// {
	// 	t_ast	*temp;

	// 	temp = ast_root;
	// 	ast_root = ast_root->right;
	// 	free_ast(temp);
	// }
	free_ast(ast_root);
}
