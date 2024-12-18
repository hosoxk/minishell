/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:15:46 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/17 15:15:47 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token **token_list)
{
	t_token *current = *token_list;
	while (current)
	{
		printf("Token: %s, Type: %d\n", current->value, current->type);
		current = current->next;
	}
}

bool	is_special_case(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '$' || c == '\'' || c == '\"')
		return (true);
	return (false);
}

void	print_ast(t_ast *node, int depth)
{
	if (!node)
		return ;
	for (int i = 0; i < depth; i++)
		printf("  ");
	printf("Node: ");
	switch (node->type)
	{
		case (PIPE):
			printf("PIPE\n");
			break ;
		case (REDIRECT_IN):
			printf("REDIRECT_IN, file: %s\n", node->file);
			break ;
		case (REDIRECT_OUT):
			printf("REDIRECT_OUT, file: %s\n", node->file);
			break ;
		case (APPEND):
			printf("APPEND, file: %s\n", node->file);
			break ;
		case (HEREDOC):
			printf("HEREDOC, file: %s\n", node->file);
			break ;
		case (WORD):
			printf("COMMAND, args: ");
			if (node->args)
			{
				for (int i = 0; node->args[i]; i++)
					printf("%s ", node->args[i]);
			}
			printf("\n");
			break ;
		default :
			printf("UNKNOWN\n");
			break ;
	}
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}
