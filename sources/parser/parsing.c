/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:12:13 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/08 16:05:22 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_argument(char ***args, int *size, int *count, const char *value)
{
	if (value == NULL)
	{
		print_error("Error: attempting to add NULL argument");
		return ;
	}
	printf("Adding argument: %s (count = %d, size = %d)\n", value, *count, *size);
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
	if((*args)[*count] == NULL)
	{
		printf(BOLD_RED"Error: ft_strdup failed for value = %s\n"RESET,
			value);
		return ;
	}
	(*count)++;
	(*args)[*count] = NULL;
	printf("Argument added succesfully. total count = %d\n", *count);
}

void set_root_ast(t_ast *node, t_ast *root)
{
	if (!node)
		return ;
	if (node == root)
		{
			node->fd_in = dup(STDIN_FILENO);
			node->fd_out = dup(STDOUT_FILENO);
		}
	set_root_ast(node->left, root);	
	set_root_ast(node->right, root);
	node->root = root;
}

t_ast	*create_ast_node(t_token_type type)
{
	t_ast	*node;

	node = malloc(sizeof (t_ast));
	if (!node)
	{
		print_error("Error: failure mallocing space for new ast_node");
		return (NULL);
	}
	node->type = type;
	node->args = NULL;
	node->file = NULL;
	node->left = NULL;
	node->right = NULL;
	node->fd_in = -1;
	node->fd_out = -1;
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
t_ast *parse_ast(t_token **tokens)
{
	t_ast	*left_node = NULL;
	t_ast *redir_node = NULL;
	t_ast *current_command = NULL;
	int size = 0, count = 0;
	bool is_prefix = 1; // flag to check if redirection is prefix or suffix
	while (*tokens)
	{
		printf(BOLD_MAGENTA"Parsing token: type = %d, value = %s\n"RESET, (*tokens)->type, (*tokens)->value);
		if ((*tokens)->type == PIPE)
			return create_pipe_node(left_node, tokens);
		else if ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT || (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
		{
			redir_node = create_redirection_node(tokens);
			if (!redir_node)
				return (print_error("Failed to create redirection node"), left_node);
			printf("Attaching redirection node: type = %d, file = %s\n", redir_node->type, redir_node->file);
			if (is_prefix)
			{
				if (!current_command)
				{
					current_command = create_ast_node(WORD);
					left_node = current_command;
				}
			attach_redirection_to_command(current_command ? current_command : (current_command = create_ast_node(WORD)), redir_node, 1);
			is_prefix = 0;
			}
			else
				attach_redirection_to_command(current_command, redir_node, 0);
		}	
		else if ((*tokens)->type == WORD)
		{
			if (!current_command || current_command->type != WORD)
			{
				current_command = create_command_node(left_node, tokens, &size, &count);
				left_node = current_command;
			}
			else
			{
				add_argument(&current_command->args, &size, &count, (*tokens)->value);
				printf("Tokens processed: type = %d, value = %s\n", (*tokens)->type, (*tokens)->value);
				*tokens = (*tokens)->next;
			}
			is_prefix = 0; // switch to suffix after first WORD token
		}
		else
			*tokens = (*tokens)->next;
	}
	if (left_node)
		printf("Returning COMMAND node\n");
	return left_node;
}
