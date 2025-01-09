#include "../minishell.h"

static bool	check_pipe(t_token *token, t_token *prev_token)
{
	if (!prev_token)
		return (print_error("Syntax error: unexpected pipe at the beginning"),
			false);
	if (!token->next || token->next->type == PIPE)
		return (print_error("Syntax error: invalid pipe sequence"), false);
	if (!token->next || token->next->type != WORD)
		return (print_error("Syntax error: missing command after pipe"), false);
	return (true);
}

static bool	check_redir(t_token *token)
{
	if (!token->next || token->next->type != WORD)
		return (print_error("Syntax error: missing target for redirection"),
			false);
	return (true);
}

bool	validate_token_sequence(t_token *tokens)
{
	t_token	*prev_token;

	prev_token = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!check_pipe(tokens, prev_token))
				return (false);
		}
		else if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
					tokens->type == APPEND || tokens->type == HEREDOC)
			if (!check_redir(tokens))
				return (false);
		prev_token = tokens;
		tokens = tokens->next;
	}
	return (true);
}
