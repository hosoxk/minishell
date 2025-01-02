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

void    add_token_to_list(t_token **token_list, char *value, t_token_type type)
{
	t_token *new_token;
	t_token *current;

	if (!token_list || !value)
		return ;
	current = NULL;
	new_token = ft_calloc(1, sizeof(t_token)); // valgrind issue also happens here? lol //TODO
	if (!new_token)
	{
		print_error("Failure mallocing space for new_token");
		return ;
	}
	new_token->value = NULL;
	new_token->value = ft_strdup(value); // valgrind issue happens here?? //TODO
	if (!new_token->value)
		return (free(new_token));
	new_token->type = type;
	new_token->next = NULL;
	if (*token_list == NULL)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

void     handle_quoted_str(char **line, t_token **token_list)
{
	char    quote_char;
	char    *start;
	char    *quoted_str;

	quote_char = **line;
	(*line)++; //skips opening quote
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
		(*line)++; //skips closing quote
	}
	else
		print_error("Error: unmatched quote");
}

void     handle_redirect(char **line, t_token **token_list)
{
	char    special[3];

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
