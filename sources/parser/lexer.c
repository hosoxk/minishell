/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:43:38 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/03 15:05:03 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	Allocate space for a new t_token, if token_list is present,
 *	token iterates to the end of the list
 */

void    add_token_to_list(t_token **token_list, char *value, t_token_type type)
{
	t_token *new_token;
	t_token *current;

	if (!token_list || !value)
		return ;
	current = NULL;
	new_token = malloc(sizeof (t_token)); // valgrind issue also happens here? lol //TODO
	if (!new_token)
	{
		print_error("Failure mallocing space for new_token");
		return ;
	}
	new_token->value = NULL;
	new_token->type = type;
	new_token->next = NULL;
	new_token->value = ft_strdup(value); // valgrind issue happens here?? //TODO
	if (!new_token->value)
		return (free(new_token));
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

static void    handle_word(char **line, t_token **token_list)
{
        char    *start;
        char    *word;

        start = *line;
        while (**line && !is_white_space(**line) && !is_special_case(**line))
                (*line)++;
        word = ft_strndup(start, *line - start);
        add_token_to_list(token_list, word, WORD);
        free(word);
}

static void     handle_special_case(char **line, t_token **token_list)
{
        if (**line == '\'' || **line == '\"')
                handle_quoted_str(line, token_list);
        else if (**line == '|')
        {
                add_token_to_list(token_list, "|", PIPE);
                (*line)++;
        }
        else if (**line == '<' || **line == '>')
        {
                handle_redirect(line, token_list);
        }
		/*
        else if (**line == '$')
        {
                handle_var(line, token_list);
        }
		*/
}

void	lexer(char *line, t_token **token_list)
{
	if (!line)
	{
		print_error("Invalid line pointer in lexer");
		return ;
	}
	while (*line)
	{
		if (is_white_space(*line))
			line++;
		else if (is_special_case(*line))
			handle_special_case(&line, token_list);
		else
			handle_word(&line, token_list);
	}
}
