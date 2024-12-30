/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:43:38 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/30 16:18:48 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		else
			handle_word(&line, token_list);
	}
}
