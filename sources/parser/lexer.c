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

/*	Allocate space for a new t_token, if token_list is present,
 *	token iterates to the end of the list
 */

bool	add_token_to_list(t_token **token_list, char *value, t_token_type type)
{
	t_token	*new_token;
	t_token	*current;

	if (!token_list) //  || !value
		return (print_error("null parameter found for add_token_to_list"),
			false);
	current = NULL;
	new_token = malloc(sizeof (t_token));
	if (!new_token)
		return (print_error("Failure mallocing space for new_token"), false);
	new_token->type = type;
	new_token->value = NULL;
	new_token->next = NULL;
	new_token->value = ft_strdup(value);
	if (!new_token->value && value)
		return (free(new_token), print_error("Error: "),
			print_error("failure ft_strdup in add_token_to_list"), false);
	if (*token_list == NULL)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (true);
}

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
	add_token_to_list(token_list, word, WORD);
	free(word);
	return (true);
}

static bool handle_parentheses(char **line, t_token **token_list)
{
	bool return_val;

	if (**line == '(')
		return_val = add_token_to_list(token_list, NULL, PARENTHESES_OPEN);
	else
		return_val = add_token_to_list(token_list, NULL, PARENTHESES_CLOSE);
	if (return_val)
		(*line)++;
	return (return_val);
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
	{
		(*line)++;
		if (**line == '|')
		{
			(*line)++;
			return (add_token_to_list(token_list, NULL, OR));
		}
		else
			return (add_token_to_list(token_list, "|", PIPE));
	}
	if (**line == '&')
	{
		(*line)++;
		if (**line == '&')
		{
			(*line)++;
			return (add_token_to_list(token_list, NULL, AND));
		}
		else
			return (false);
	}
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
