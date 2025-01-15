/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:43:38 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/08 17:04:02 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	handle_word(char **line, t_token **token_list)
{
	char	*start;
	char	*word;

	if (!line || !token_list)
		return (print_error("null parameter found for handle_word"), false);
	start = *line;
	while (**line && !is_white_space(**line) && !is_special_case(**line))
		(*line)++;
	word = ft_strndup(start, *line - start);
	if (!add_token_to_list(token_list, word, WORD))
		return (free(word), false);
	free(word);
	return (true);
}

static bool	handle_special_case(char **line, t_token **token_list)
{
	if (**line == '\'' || **line == '\"')
		return (handle_quoted_str(line, token_list));
	if (ft_strchr("()", **line))
		return (handle_parentheses(line, token_list));
	if (**line == '<' || **line == '>')
		return (handle_redirect(line, token_list));
	if (**line == '|')
		return (handle_pipe(line, token_list));
	if (**line == '&')
		return (handle_ampersand(line, token_list));
	return (true);
}

bool	lexer(char *line, t_token **token_list)
{
	if (!line)
	{
		print_error("Invalid line pointer in lexer");
		return (false);
	}
	while (*line)
	{
		if (is_white_space(*line))
			line++;
		else if (is_special_case(*line))
		{
			if (!handle_special_case(&line, token_list))
				return (false);
		}
		else
			if (!handle_word(&line, token_list))
				return (false);
	}
	return (true);
}
