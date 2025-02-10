/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_lexer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:20:34 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/02/10 09:08:57 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	handle_parentheses(char **line, t_token **token_list, char **env)
{
	bool	return_val;

	if (**line == '(')
		return_val = add_token_to_list(token_list, NULL, PARENTHESES_OPEN, env);
	else
		return_val = add_token_to_list(token_list, NULL, PARENTHESES_CLOSE, env);
	if (return_val)
		(*line)++;
	return (return_val);
}

bool	handle_pipe(char **line, t_token **token_list, char **env)
{
	(*line)++;
	if (**line == '|')
	{
		(*line)++;
		return (add_token_to_list(token_list, NULL, OR, env));
	}
	else
		return (add_token_to_list(token_list, "|", PIPE, env));
}

bool	handle_ampersand(char **line, t_token **token_list, char **env)
{
	(*line)++;
	if (**line == '&')
	{
		(*line)++;
		return (add_token_to_list(token_list, NULL, AND, env));
	}
	print_error_exit_status("Syntax error: unexpected error near &", 258, env);
	return (false);
}
