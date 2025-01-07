/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:48:39 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/06 12:30:00 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*create_pipe_node(t_ast *left_node, t_token **tokens)
{
	t_ast	*pipe_node;

	printf("Found PIPE token, creating PIPE node\n");
	pipe_node = create_ast_node(PIPE);
	if (!pipe_node)
		return (print_error("Failed to allocate memory for PIPE node"), NULL);
	pipe_node->left = left_node;
	*tokens = (*tokens)->next;
	printf("Moving to parse right side of PIPE\n");
	pipe_node->right = parse_ast(tokens);
	return (pipe_node);
}
/*
void	create_heredoc_node(t_token **tokens, t_ast *redir_node)
{
	char	*heredoc_content;
	size_t	len;

	heredoc_content = NULL;
	len = 0;
	while (*tokens && (*tokens)->type != WORD)
	{
		size_t	token_len;

		token_len = ft_strlen((*tokens)->value);
		heredoc_content = ft_realloc(heredoc_content, len, len + token_len + 1);
		if (!heredoc_content)
		{
			print_error("Failed to reallocate memory for heredoc content"RESET);
			return ;
		}
		ft_strcpy(heredoc_content + len, (*tokens)->value);
		len += token_len;
		*tokens = (*tokens)->next;
	}
	if (*tokens && ft_strcmp((*tokens)->value, "EOF") == 0)
		*tokens = (*tokens)->next;
	redir_node->file = heredoc_content;
	printf("Attached heredoc content to REDIRECTION node: %s\n", redir_node->file);
}
*/
void	attach_redirection_to_command(t_ast *command, t_ast *redir)
{
	if (!redir)
	{
		print_error("Error: redirection node is NULL");
		return ;
	}
	if (!command)
		command = create_ast_node(WORD);
	if (!command->left)
		command->left = redir;
	else
	{
		t_ast	*temp;

		temp = command->left;
		while (temp->right)
			temp = temp->right; // find the last redirection
		temp->right = redir;
	}
	printf("Attached redir to command\n");
}

t_ast	*create_redirection_node(t_token **tokens)
{
	t_ast	*redir_node;

	print_tokens(tokens);
	if (!tokens || !(*tokens))
		return (print_error("Null token pointer in create_redirection_node"), NULL);
	printf("Found REDIRECTION token: %d, creating REDIRECTION node\n", (*tokens)->type);
	redir_node = create_ast_node((*tokens)->type);
	if (!redir_node)
		return (print_error("Failed to allocate memory for REDIRECTION node"), NULL);
	*tokens = (*tokens)->next; // move past the redirection token
	if ((*tokens) && (*tokens)->type == WORD)
	{
		redir_node->file = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next; // move past file token
	}
	else
		return (print_error("Missing file for redirection"), NULL);
	return (redir_node);
}

t_ast	*create_command_node(t_ast *command_node, t_token **tokens, int *size, int *count)
{
	t_ast	*node;

	if (command_node && command_node->type == WORD)
		node = command_node;
	else
		node = create_ast_node(WORD);
	add_argument(&node->args, size, count, (*tokens)->value);
	printf("Adding argument to COMMAND node: %s\n", (*tokens)->value);
	*tokens = (*tokens)->next;
	return (node);
	/*
	if (!command_node)
	{
		printf("Creating COMMAND node\n");
		command_node = create_ast_node(WORD);
		if (!command_node)
			return (print_error("Failed to allocate memory for COMMAND node"), NULL);
	}
	printf("Adding argument to COMMAND node: %s\n", (*tokens)->value);
	add_argument(&(command_node->args), size, count, (*tokens)->value);
	return (command_node);
	*/
}
