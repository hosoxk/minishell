#include "minishell.h"

t_ast	*create_pipe_node(t_ast *left_node, t_token **tokens)
{
	t_ast	*pipe_node;

	printf("Found PIPE token, creating PIPE node\n");
	pipe_node = create_ast_node(PIPE);
	if (!pipe_node)
		return (printf("Failed to allocate memory for PIPE node\n"), NULL);
	pipe_node->left = left_node;
	*tokens = (*tokens)->next;
	if (!*tokens)
		return (printf("Syntax error: unexpected end of input after PIPE\n"), NULL);
	printf("Moving to parse right side of PIPE\n");
	pipe_node->right = parse_ast(tokens);
	return (pipe_node);
}

t_ast	*create_redirection_node(t_ast *left_node, t_token **tokens)
{
	t_ast	*redir_node;

	printf("Found REDIRECTION token: %d, creating REDIRECTION node\n", (*tokens)->type);
	redir_node = create_ast_node((*tokens)->type);
	if (!redir_node)
		return (printf(BOLD_RED"Failed to allocate memory for REDIRECTION node\n"RESET), NULL);
	*tokens = (*tokens)->next; // move past the redirection token
	if (redir_node->type == HEREDOC)
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
				return (printf("Failed to reallocate memory for heredoc content\n"), NULL);
			ft_strcpy(heredoc_content + len, (*tokens)->value);
			len += token_len;
			*tokens = (*tokens)->next;
		}
		if (*tokens && ft_strcmp((*tokens)->value, "EOF") == 0)
			*tokens = (*tokens)->next;
		redir_node->file = heredoc_content;
		printf("Attached heredoc content to REDIRECTION node: %s\n", redir_node->file);
	}
	else if (*tokens && (*tokens)->type == WORD)
	{
		redir_node->file = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	else
		return (printf(BOLD_RED"Syntax error: missing file for redirection\n"RESET), NULL);
	redir_node->left = left_node;
	return (redir_node);
}

t_ast	*create_command_node(t_ast *command_node, t_token **tokens, int *size, int *count)
{
	if (!command_node)
	{
		printf("Creating COMMAND node\n");
		command_node = create_ast_node(WORD);
		if (!command_node)
			return (printf(BOLD_RED"Failed to allocate memory for COMMAND node\n"RESET), NULL);
	}
	printf("Adding argument to COMMAND node: %s\n", (*tokens)->value);
	add_argument(&(command_node->args), size, count, (*tokens)->value);
	return (command_node);
}
