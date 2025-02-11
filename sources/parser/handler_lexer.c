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

bool	handle_quoted_str(char **line, t_token **token_list,
		char *absoluut_begin, char **env)
{
	char			quote_char;
	char			*start;
	char			*quoted_str;
	t_quoted_struct	*s;

	quote_char = **line;
	(*line)++;
	start = *line;
	while (**line && **line != quote_char)
		(*line)++;
	if (**line != quote_char)
		return (print_error_exit_status("Error: unmatched quote", 258, env),
			false);
	quoted_str = ft_strndup(start, *line - start);
	if (!quoted_str)
		return (print_error_status("Error: failure malloc in \
			handle_quoted_str", env), false);
	if (!((absoluut_begin == start || *(start - 2) == ' ')))
		quote_char = '?';
	s = init_quoted_struct(line, quoted_str, quote_char, token_list);
	if (!s)
		return (free(quoted_str), false);
	s->env = env;
	return (add_quoted_token(s), true);
}

bool	handle_parentheses(char **line, t_token **token_list, char **env)
{
	bool	return_val;

	if (**line == '(')
		return_val = add_token(token_list, NULL, PARENTHESES_OPEN, env);
	else
		return_val = add_token(token_list, NULL, PARENTHESES_CLOSE,
				env);
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
		return (add_token(token_list, NULL, OR, env));
	}
	else
		return (add_token(token_list, "|", PIPE, env));
}

bool	handle_ampersand(char **line, t_token **token_list, char **env)
{
	(*line)++;
	if (**line == '&')
	{
		(*line)++;
		return (add_token(token_list, NULL, AND, env));
	}
	print_error_exit_status("Syntax error: unexpected error near &", 258, env);
	return (false);
}
