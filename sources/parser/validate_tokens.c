/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:08:12 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/14 13:33:09 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	check_pipe(t_token *token, t_token *prev_token)
{
	bool	is_valid;
	
	is_valid = false;
	if (!prev_token)
		print_error("Syntax error: unexpected pipe at the beginning");
	else if (!token->next || token->next->type == PIPE)
		print_error("Syntax error: invalid pipe sequence");
	else if (!token->next || token->next->type != WORD)
		print_error("Syntax error: missing command after pipe");
	else
		is_valid = true;
	if (!is_valid)
		g_exit_status = 258;
	return (is_valid);
}

static bool	check_redir(t_token *token)
{
	if (!token->next || token->next->type != WORD)
	{
		g_exit_status = 258;
		print_error("Syntax error: missing target for redirection");
		return (false);
	}
	return (true);
}

bool	validate_token_sequence(t_token *tokens)
{
	t_token	*prev_token;
	int		depth;
	bool	is_valid;

	is_valid = true;
	prev_token = NULL;
	depth = 0;
	while (tokens)
	{
		if (tokens->type == PIPE)
			is_valid = check_pipe(tokens, prev_token);
		else if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT || \
				tokens->type == APPEND || tokens->type == HEREDOC)
			is_valid = check_redir(tokens);
		if (!is_valid)
			return (false);
		if (tokens->type == PARENTHESES_OPEN)
			depth++;
		if (tokens->type == PARENTHESES_CLOSE)
			depth--;
		prev_token = tokens;
		tokens = tokens->next;
	}
	return (depth == 0);
}
