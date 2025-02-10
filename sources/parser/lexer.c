/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:09:42 by kvanden-          #+#    #+#             */
/*   Updated: 2025/02/05 15:09:42 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	handle_word(char **line, t_token **token_list, char *absoluut_begin, char **env)
{
	char	*start;
	char	*word;

	if (!line || !token_list)
		return (print_error("null parameter found for handle_word"), false);
	start = *line;
	while (**line && !is_white_space(**line) && !is_special_case(**line))
		(*line)++;
	word = ft_strndup(start, *line - start);
	if (!(absoluut_begin == start || *(start -1) == ' '))
	{
		if (!add_token_to_list(token_list, word, FAKE_QUOTED_STRING, env))
			return (free(word), false);
	}
	else if (!add_token_to_list(token_list, word, WORD, env))
		return (free(word), false);
	free(word);
	return (true);
}

static bool	handle_special_case(char **line, t_token **token_list,
	char *absoluut_begin, char **env)
{
	if (**line == '\'' || **line == '\"')
		return (handle_quoted_str(line, token_list, absoluut_begin, env));
	if (ft_strchr("()", **line))
		return (handle_parentheses(line, token_list, env));
	if (**line == '<' || **line == '>')
		return (handle_redirect(line, token_list, env));
	if (**line == '|')
		return (handle_pipe(line, token_list, env));
	if (**line == '&')
		return (handle_ampersand(line, token_list, env));
	return (true);
}

bool	lexer(char *line, t_token **token_list, char **env)
{
	char	*start;

	if (!line)
	{
		print_error("Invalid line pointer in lexer");
		return (false);
	}
	start = line;
	while (*line)
	{
		if (is_white_space(*line))
			line++;
		else if (is_special_case(*line))
		{
			if (!handle_special_case(&line, token_list, start, env))
				return (false);
		}
		else if (!handle_word(&line, token_list, start, env))
			return (false);
	}
	return (true);
}
