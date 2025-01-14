/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:56:36 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/14 09:03:43 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	handle_quoted_str(char **line, t_token **token_list)
{
	char	quote_char;
	char	*start;
	char	*quoted_str;


	if (!line || !token_list)
		return (print_error("null parameter found for handle_quoted_str"),
			false);
	if (!line || !token_list)
		return (print_error("null parameter found for handle_quoted_str"),
			false);
	quote_char = **line;
	(*line)++;
	start = *line;
	while (**line && **line != quote_char)
		(*line)++;
	if (**line == quote_char)
	{
		quoted_str = ft_strndup(start, *line - start);
		if (quote_char == '\'' && **line == '\'')
		{
			if (!add_token_to_list(token_list, quoted_str, QUOTED_STRING))
				return (false);
		}
		else
			if (!add_token_to_list(token_list, quoted_str, DOUBLE_QUOTED_STRING))
				return (false);
		free(quoted_str);
		(*line)++;
	}
	else
		return (print_error("Error: unmatched quote"), false);
	return (true);
}

bool	handle_redirect(char **line, t_token **token_list)
{
	char	special[3];

	if (!line || !(*line))
		return (print_error("Handler received a null line pointer"), false);
	// if (!(*line + 1) || !*(*line + 1))
	// 	return (print_error("Handler exprected target for redirection"), false);
	special[0] = **line;
	special[1] = '\0';
	special[2] = '\0';
	if ((**line == '<' && *(*line + 1) == '<') || (**line == '>' && *(*line + 1) == '>'))
	{
		special[1] = *(*line + 1);
		if (add_token_to_list(token_list, special, **line == '<' ? HEREDOC : APPEND))
			(*line) += 2;
		else
			return (false);
		if (add_token_to_list(token_list, special, **line == '<' ? HEREDOC : APPEND))
			(*line) += 2;
		else
			return (false);
	}
	else
	{
		if (add_token_to_list(token_list, special, **line == '<' ? REDIRECT_IN : REDIRECT_OUT))
			(*line)++;
		else
			return (false);
		if (add_token_to_list(token_list, special, **line == '<' ? REDIRECT_IN : REDIRECT_OUT))
			(*line)++;
		else
			return (false);
	}
	return (true);
	return (true);
}

/*
void     handle_var(char **line, t_token **token_list)
{
	char    *start;
	char    *variable;

	start = ++(*line);
	while (**line && (ft_isalnum(**line) || **line == '_'))
		(*line)++;
	variable = ft_strndup(start, *line - start);
	add_token_to_list(token_list, variable, VARIABLE);
	free(variable);
}
*/

