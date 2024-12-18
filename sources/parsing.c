#include "minishell.h"

static void	add_argument(char ***args, int *size, int *count, const char *value)
{
	if (*args == NULL)
	{
		// initial allocation
		*size = 8; // start with space for 8 arguments
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
		*size *= 2;
		*args = ft_realloc(*args, old_size, (*size) * sizeof (char *));
		if (!*args)
			return ;
	}
	// add the argument to the array
	(*args)[*count] = ft_strdup(value);
	(*count)++;
	(*args)[*count] = NULL; // null-terminate
}

static t_ast	*create_ast_node(t_token_type type)
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
		// Handle PIPE token
		if ((*tokens)->type == PIPE)
		{
			t_ast	*pipe_node;
			printf("Found PIPE token, creating PIPE node\n");
			// Create PIPE node
			pipe_node = create_ast_node(PIPE);
			pipe_node->left = left_node;
			*tokens = (*tokens)->next; // Move past PIPE
			printf("Moving to parse right side of PIPE\n");
			// Recursively parse the right side of the pipe
			pipe_node->right = parse_ast(tokens);
			return pipe_node;
		}
		// Handle Redirection tokens (<, >, >>, heredoc)
		else if ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT
				|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
		{
			t_ast	*redir_node;

			printf("Found REDIRECTION token: %d, creating REDIRECTION node\n", (*tokens)->type);
			redir_node = create_ast_node((*tokens)->type);
			*tokens = (*tokens)->next; // Move past redirection
			// If it's HEREDOC, handle heredoc content
			if (redir_node->type == HEREDOC)
			{
				char *heredoc_content;
				size_t len;

				heredoc_content = NULL;
				len = 0;
				while (*tokens && (*tokens)->type != WORD)
				{
					size_t token_len;

					token_len = ft_strlen((*tokens)->value);
					heredoc_content = ft_realloc(heredoc_content, len, len + token_len + 1);
					if (!heredoc_content)
						return NULL;
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
				return printf("Syntax error: missing file for redirection\n"), NULL;
			// Attach previous node to redirection node (left child)
			redir_node->left = left_node;
			left_node = redir_node;  // Update left_node to ensure correct connection
		}
		// Handle COMMAND node (any other token is treated as part of a command)
		else
		{
			if (!left_node)
			{
				printf("Creating COMMAND node\n");
				left_node = create_ast_node(WORD);
			}
			printf("Adding argument to COMMAND node: %s\n", (*tokens)->value);
			add_argument(&(left_node->args), &size, &count, (*tokens)->value);
			*tokens = (*tokens)->next;
		}
	}
	if (left_node)
	{
		printf("Returning COMMAND node\n");
	}
	return left_node;
}
