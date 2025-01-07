/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:56:36 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/30 18:23:15 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void     handle_quoted_str(char **line, t_token **token_list)
{
	char    quote_char;
	char    *start;
	char    *quoted_str;

	quote_char = **line;
	(*line)++;
	start = *line;
	while (**line && **line != quote_char)
		(*line)++;
	if (**line == quote_char)
	{
		quoted_str = ft_strndup(start, *line - start);
		if (quote_char == '\'' && **line == '\'')
			add_token_to_list(token_list, quoted_str, QUOTED_STRING);
		else
			add_token_to_list(token_list, quoted_str, DOUBLE_QUOTED_STRING);
		free(quoted_str);
		(*line)++;
	}
		print_error("Error: unmatched quote");
}

void     handle_redirect(char **line, t_token **token_list)
{
	char    special[3];

	if (!line || !(*line))
	{
		print_error("Handler received a null line pointer");
		return ;
	}
	if (!(*line + 1) || !**(line + 1))
	{
		print_error("Handler exprected target for redirection");
		return ;
	}
	special[0] = **line;
	special[1] = '\0';
	special[2] = '\0';
	if ((**line == '<' && *(*line + 1) == '<') ||
			(**line == '>' && *(*line + 1) == '>'))
	{
		special[1] = *(*line + 1);
		add_token_to_list(token_list, special, **line == '<' ? HEREDOC : APPEND);
		(*line) += 2;
	}
	else
	{
		add_token_to_list(token_list, special, **line == '<' ? REDIRECT_IN : REDIRECT_OUT);
		(*line)++;
	}
}

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
