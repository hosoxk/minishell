/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:12:13 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/18 18:12:45 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_argument(char ***args, int *size, int *count, const char *value)
{
	if (*args == NULL)
	{
		// initial allocation
		*size = ARG_SIZE;
		*args = malloc(sizeof (char *) * (*size));
		if (!*args)
			return ;
		*count = 0;
	}
	else if (*count >= *size)
	{
		// resize the array when full
		int	old_size;

		old_size = (*size) * sizeof (char *);
		*size += ARG_SIZE;
		*args = ft_realloc(*args, old_size, (*size) * sizeof (char *));
		if (!*args)
			return ;
	}
	// add the argument to the array
	(*args)[*count] = ft_strdup(value);
	(*count)++;
	(*args)[*count] = NULL; // null-terminate
}

t_ast	*create_ast_node(t_token_type type)
{
	t_ast	*node;

	node = malloc(sizeof (t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->file = NULL;
	node->left = NULL;
	node->right = NULL;
	printf("Created AST node: type = %d\n", type);
	return (node);
}

/*
 *	Abstract Syntax Tree: tree-like data structure.
 *	Each node represents an operation (pipe, redir, ...), command with its arguments,
 *	or connections (left and right children).
 *	Child nodes represent components or arguments of that construct.
 *	Left child: the "upstream" operation or command, meaning the input for the current node.
 *	Right child: the "downstream" operation or command, meaning what happens next.
 *
 *	Step 1) Initialize
 *		left_node tracks the subtree constructed so far.
 *	Step 2) Token iteration
 *		Each token is processed sequentially in this function.
 */

t_ast	*parse_ast(t_token **tokens)
{
	t_ast	*left_node;
	int	size;
	int	count;

	size = 0;
	count = 0;
	left_node = NULL;
	while (*tokens)
	{
		printf("Parsing token: type = %d, value = %s\n", (*tokens)->type, (*tokens)->value);
		if ((*tokens)->type == PIPE)
			return (create_pipe_node(left_node, tokens));
		else if ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT
				|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
		{
			left_node = create_redirection_node(left_node, tokens);
			if (!left_node)
				return (NULL); // handle error in redirection parsing //TODO?
		}
		else
		{
			left_node = create_command_node(left_node, tokens, &size, &count);
			*tokens = (*tokens)->next;
		}
	}
	if (left_node)
	{
		printf("Returning COMMAND node\n");
	}
	return left_node;
}
