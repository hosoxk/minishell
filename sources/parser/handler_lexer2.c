/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_lexer2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:56:36 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/02/12 10:12:37 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	add_quoted_token(t_quoted_struct *s)
{
	if (s->quote_char == '\'')
	{
		if (!add_token(s->token_list, s->quoted_str, QOUTED_STR, s->env))
			return (false);
	}
	else if (s->quote_char == '\"') /// '\''
	{
		if (!add_token(s->token_list, s->quoted_str, D_QOUTED_STR, s->env))
			return (false);
	}
	else
	{
		if ((**s->line) == '\'')
		{
			if (!add_token(s->token_list, s->quoted_str,
					FAKE_QOUTED_STR, s->env))
				return (false);
		}
		else if (!add_token(s->token_list, s->quoted_str,
				FAKE_D_QOUTED_STR, s->env))
			return (false);
	}
	free(s->quoted_str);
	(*s->line)++;
	return (free(s), true);
}

t_quoted_struct	*init_quoted_struct(char **line, char *quoted_str,
		char quote_char, t_token **token_list)
{
	t_quoted_struct	*s;
	////???

	s = malloc(sizeof(t_quoted_struct));
	if (!s)
		return (NULL);
	s->line = line;
	s->quoted_str = quoted_str;
	s->quote_char = quote_char;
	s->token_list = token_list;
	return (s);
}

static bool	handle_heredoc_append(char **line, char *special,
		t_token **token_list, char **env)
{
	if ((**line == '<' && *(*line + 1) == '<'))
	{
		special[1] = '<';
		if (!add_token(token_list, special, HEREDOC, env))
			return (false);
		(*line) += 2;
	}
	else if ((**line == '>' && *(*line + 1) == '>'))
	{
		special[1] = '>';
		if (!add_token(token_list, special, APPEND, env))
			return (false);
		(*line) += 2;
	}
	else
		return (false);
	return (true);
}

static bool	handle_single_redirect(char **line, char *special,
		t_token **token_list, char **env)
{
	if (**line == '<')
	{
		if (!add_token(token_list, special, REDIRECT_IN, env))
			return (false);
		(*line)++;
	}
	else if (**line == '>')
	{
		if (!add_token(token_list, special, REDIRECT_OUT, env))
			return (false);
		(*line)++;
	}
	else
		return (false);
	return (true);
}

bool	handle_redirect(char **line, t_token **token_list, char **env)
{
	char	special[3];

	if (!line || !(*line))
		return (print_error("Handler received a null line pointer"), false);
	special[0] = **line;
	special[1] = '\0';
	special[2] = '\0';
	if (!handle_heredoc_append(line, special, token_list, env))
		if (!handle_single_redirect(line, special, token_list, env))
			return (false);
	return (true);
}
