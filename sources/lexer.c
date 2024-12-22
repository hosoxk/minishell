/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:43:38 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/16 17:17:40 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_token_sequence(t_token *tokens)
{
	t_token	*prev_token;

	prev_token = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!prev_token)
				return (printf(BOLD_RED"Syntax error: unexpected pipe at the beginning\n"RESET), false);
			if (!tokens->next || tokens->next->type == PIPE)
				return (printf(BOLD_RED"Syntax error: invalid pipe sequence\n"RESET), false);
			if (!tokens->next || tokens->next->type != WORD)
				return (printf(BOLD_RED"Syntax error: missing command after pipe\n"RESET), false);
		}
		if ((tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
			tokens->type == APPEND || tokens->type == HEREDOC))
		{
			if (!tokens->next || tokens->next->type != WORD)
				return (printf(BOLD_RED"Syntax error: missing target for redirection\n"RESET), false);
		}
		if ((tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT || tokens->type == APPEND
			|| tokens->type == HEREDOC || tokens->type == PIPE) && !tokens->next)
			return (printf(BOLD_RED"Syntax error: invalid token at the beginning\n"RESET), false);
		prev_token = tokens;
		tokens = tokens->next;
	}
	return (true);
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
        else if (**line == '$')
        {
                handle_var(line, token_list);
        }
}

void	lexer(char *line, t_token **token_list)
{
	while (*line)
	{
		if (is_white_space(*line))
			line++;
		else if (is_special_case(*line))
			handle_special_case(&line, token_list);
			//printf("%c is a special character\n", *line++);
		else
			handle_word(&line, token_list);
	}
}
